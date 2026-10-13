// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Interaction/HorrorInteractionComponent.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Engine/OverlapResult.h"

UHorrorInteractionComponent::UHorrorInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f; // Trace at 20Hz for optimal performance
}

void UHorrorInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AHorrorCharacter>(GetOwner());
}

void UHorrorInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PerformTrace();
}

void UHorrorInteractionComponent::PerformTrace()
{
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<AHorrorCharacter>(GetOwner());
		if (!OwnerCharacter.IsValid())
		{
			ClearFocus();
			return;
		}
	}

	UCameraComponent* CameraComp = OwnerCharacter->GetFirstPersonCameraComponent();
	if (!CameraComp)
	{
		ClearFocus();
		return;
	}

	const FVector TraceStart = CameraComp->GetComponentLocation();
	const FVector CameraForward = CameraComp->GetForwardVector();
	const FVector TraceEnd = TraceStart + (CameraForward * InteractionDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter.Get());

	AActor* BestCandidate = nullptr;

	// 1. Direct raycast straight out of camera center
	FHitResult DirectHit;
	if (GetWorld()->LineTraceSingleByChannel(DirectHit, TraceStart, TraceEnd, TraceChannel, QueryParams))
	{
		if (DirectHit.GetActor() && DirectHit.GetActor()->GetClass()->ImplementsInterface(UHorrorInteractable::StaticClass()))
		{
			BestCandidate = DirectHit.GetActor();
		}
	}

	// 2. Multi-Sphere Sweep (35cm radius):
	// Unlike SweepSingle, SweepMulti will NOT get stopped by the floor or desk, so small keys/props are easily caught
	if (!BestCandidate)
	{
		TArray<FHitResult> SweepHits;
		const bool bAnySweepHit = GetWorld()->SweepMultiByChannel(
			SweepHits,
			TraceStart,
			TraceEnd,
			FQuat::Identity,
			TraceChannel,
			FCollisionShape::MakeSphere(35.0f),
			QueryParams
		);

		if (bAnySweepHit)
		{
			for (const FHitResult& Hit : SweepHits)
			{
				AActor* Candidate = Hit.GetActor();
				if (Candidate && Candidate->GetClass()->ImplementsInterface(UHorrorInteractable::StaticClass()))
				{
					BestCandidate = Candidate;
					break;
				}
			}
		}
	}

	// 3. Proximity Cone Check:
	// If Cross is standing near a table or floor containing an interactable and looking roughly in its direction
	// (within ~35 degrees of center view), lock onto it immediately
	if (!BestCandidate)
	{
		TArray<FOverlapResult> Overlaps;
		const FCollisionShape ProximitySphere = FCollisionShape::MakeSphere(InteractionDistance);
		if (GetWorld()->OverlapMultiByChannel(Overlaps, TraceStart, FQuat::Identity, TraceChannel, ProximitySphere, QueryParams))
		{
			float BestScore = -1.0f;
			for (const FOverlapResult& Overlap : Overlaps)
			{
				AActor* Candidate = Overlap.GetActor();
				if (Candidate && Candidate != OwnerCharacter.Get() && Candidate->GetClass()->ImplementsInterface(UHorrorInteractable::StaticClass()))
				{
					const FVector DirToTarget = (Candidate->GetActorLocation() - TraceStart).GetSafeNormal();
					const float Dot = FVector::DotProduct(CameraForward, DirToTarget);

					// Within ~35 degrees cone of gaze (Dot > 0.82)
					if (Dot > 0.82f && Dot > BestScore)
					{
						// Check line of sight so we don't interact through walls
						FHitResult OcclusionHit;
						const bool bOccluded = GetWorld()->LineTraceSingleByChannel(OcclusionHit, TraceStart, Candidate->GetActorLocation(), TraceChannel, QueryParams);
						if (!bOccluded || OcclusionHit.GetActor() == Candidate)
						{
							BestScore = Dot;
							BestCandidate = Candidate;
						}
					}
				}
			}
		}
	}

	// 4. Update focus state
	if (BestCandidate)
	{
		if (IHorrorInteractable::Execute_CanInteract(BestCandidate, OwnerCharacter.Get()))
		{
			// If already focusing on this exact actor, just refresh prompt
			if (CurrentFocusedActor.Get() == BestCandidate)
			{
				CurrentPrompt = IHorrorInteractable::Execute_GetInteractionPrompt(BestCandidate, OwnerCharacter.Get());
				if (GEngine && !CurrentPrompt.IsEmpty())
				{
					GEngine->AddOnScreenDebugMessage(1001, 0.1f, FColor::Yellow, FString::Printf(TEXT("[E] %s"), *CurrentPrompt.ToString()));
				}
				return;
			}

			// Clear previous focus
			if (CurrentFocusedActor.IsValid())
			{
				ClearFocus();
			}

			// Assign new focus
			CurrentFocusedActor = BestCandidate;
			CurrentInteractable.SetObject(BestCandidate);
			CurrentInteractable.SetInterface(Cast<IHorrorInteractable>(BestCandidate));

			IHorrorInteractable::Execute_OnFocusGained(BestCandidate, OwnerCharacter.Get());
			CurrentPrompt = IHorrorInteractable::Execute_GetInteractionPrompt(BestCandidate, OwnerCharacter.Get());

			if (GEngine && !CurrentPrompt.IsEmpty())
			{
				GEngine->AddOnScreenDebugMessage(1001, 0.1f, FColor::Yellow, FString::Printf(TEXT("[E] %s"), *CurrentPrompt.ToString()));
			}

			OnInteractableFocused.Broadcast(BestCandidate, CurrentPrompt);
			return;
		}
	}

	// No valid interactable found in front of Cross
	if (CurrentFocusedActor.IsValid())
	{
		ClearFocus();
	}
}

void UHorrorInteractionComponent::ClearFocus()
{
	if (CurrentFocusedActor.IsValid())
	{
		if (CurrentFocusedActor->GetClass()->ImplementsInterface(UHorrorInteractable::StaticClass()))
		{
			IHorrorInteractable::Execute_OnFocusLost(CurrentFocusedActor.Get(), OwnerCharacter.Get());
		}
	}

	CurrentFocusedActor = nullptr;
	CurrentInteractable = nullptr;
	CurrentPrompt = FText::GetEmpty();

	OnInteractableUnfocused.Broadcast();
}

bool UHorrorInteractionComponent::TryInteract()
{
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<AHorrorCharacter>(GetOwner());
		if (!OwnerCharacter.IsValid())
		{
			return false;
		}
	}

	// If focus is currently null, run PerformTrace right now so pressing E never misses an object
	if (!CurrentFocusedActor.IsValid())
	{
		PerformTrace();
	}

	if (CurrentFocusedActor.IsValid() && CurrentFocusedActor->GetClass()->ImplementsInterface(UHorrorInteractable::StaticClass()))
	{
		if (IHorrorInteractable::Execute_CanInteract(CurrentFocusedActor.Get(), OwnerCharacter.Get()))
		{
			IHorrorInteractable::Execute_Interact(CurrentFocusedActor.Get(), OwnerCharacter.Get());
			
			// Refresh trace state immediately after interaction
			PerformTrace();
			return true;
		}
	}

	return false;
}
