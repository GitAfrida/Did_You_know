// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Interaction/HorrorInspectable.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Variant_Horror/Investigation/MilaDialogueSubsystem.h"
#include "Variant_Horror/Investigation/HorrorObjectiveSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AHorrorInspectable::AHorrorInspectable()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InspectableMesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AHorrorInspectable::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent)
	{
		MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

		// If no mesh is assigned, provide a default 3D model: wrench/tool for pickups, paper document for clues
		if (!MeshComponent->GetStaticMesh())
		{
			const TCHAR* FallbackPath = bIsPickup
				? TEXT("/Game/MafiaBarnPack/Meshes/Props/Tools/SM_Wrench.SM_Wrench")
				: TEXT("/Game/MafiaBarnPack/Meshes/Props/Paper/SM_PhotoPlane1.SM_PhotoPlane1");

			UStaticMesh* DefaultMesh = LoadObject<UStaticMesh>(nullptr, FallbackPath);
			if (DefaultMesh)
			{
				MeshComponent->SetStaticMesh(DefaultMesh);
			}
		}
	}
}

bool AHorrorInspectable::CanInteract_Implementation(AHorrorCharacter* Interactor)
{
	return Interactor != nullptr;
}

FText AHorrorInspectable::GetInteractionPrompt_Implementation(AHorrorCharacter* Interactor)
{
	if (!CustomPrompt.IsEmpty())
	{
		return CustomPrompt;
	}

	if (bIsPickup)
	{
		return FText::FromString(TEXT("Take Study Key"));
	}

	return FText::FromString(TEXT("Examine Mila's Portrait"));
}

void AHorrorInspectable::OnFocusGained_Implementation(AHorrorCharacter* Interactor)
{
	if (MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(true);
	}
}

void AHorrorInspectable::OnFocusLost_Implementation(AHorrorCharacter* Interactor)
{
	if (MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(false);
	}
}

void AHorrorInspectable::Interact_Implementation(AHorrorCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 1. Play inspection sound
	if (InspectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, InspectSound, GetActorLocation());
	}

	// 2. Handle Pickup vs Examination
	if (bIsPickup)
	{
		const FName KeyToGive = !PickupItemId.IsNone() ? PickupItemId : FName("Key_Study");

		if (UHorrorInventoryComponent* InvComp = Interactor->FindComponentByClass<UHorrorInventoryComponent>())
		{
			InvComp->AddItem(KeyToGive);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(3004, 6.0f, FColor::Green, FString::Printf(TEXT("[INVENTORY] Collected: %s! You can now unlock the door."), *KeyToGive.ToString()));
		}

		Destroy();
		return;
	}

	// 3. Clue Examination: Always deliver Mila's fact and record in notebook
	const FName FactIdToUse = !TriggerMilaFactId.IsNone() ? TriggerMilaFactId : FName("Fact_MilaPortrait");
	const FText QuoteToUse = !TriggerMilaFactQuote.IsEmpty()
		? TriggerMilaFactQuote
		: FText::FromString(TEXT("Did you know that memories linger in cold places? Victor hid the key where time stands still."));
	const FText DeductionToUse = !TriggerMilaFactDeduction.IsEmpty()
		? TriggerMilaFactDeduction
		: FText::FromString(TEXT("The study key is hidden near the stopped grandfather clock."));

	// Record in world dialogue subsystem
	if (UMilaDialogueSubsystem* DialogueSubsystem = World->GetSubsystem<UMilaDialogueSubsystem>())
	{
		DialogueSubsystem->TriggerMilaFact(FactIdToUse, QuoteToUse, DeductionToUse);
	}

	// Deliver directly to player notebook component
	if (UHorrorInventoryComponent* InvComp = Interactor->FindComponentByClass<UHorrorInventoryComponent>())
	{
		InvComp->RecordMilaFact(FactIdToUse, QuoteToUse, DeductionToUse);
	}

	// Display prominent on-screen text for the player
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3001, 10.0f, FColor::Cyan, FString::Printf(TEXT("Mila: \"%s\""), *QuoteToUse.ToString()));
		GEngine->AddOnScreenDebugMessage(3002, 10.0f, FColor::Yellow, FString::Printf(TEXT("[NOTEBOOK CLUE] %s"), *DeductionToUse.ToString()));
	}

	// 4. Update objective if configured
	if (!NewObjectiveOnInspect.IsEmpty())
	{
		if (UHorrorObjectiveSubsystem* ObjSubsystem = World->GetSubsystem<UHorrorObjectiveSubsystem>())
		{
			ObjSubsystem->SetObjective(NewObjectiveOnInspect);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(3003, 7.0f, FColor::Green, FString::Printf(TEXT("[OBJECTIVE] %s"), *NewObjectiveOnInspect.ToString()));
		}
	}

	bHasBeenExamined = true;
	OnInspectableInspected.Broadcast(DocumentData);
}
