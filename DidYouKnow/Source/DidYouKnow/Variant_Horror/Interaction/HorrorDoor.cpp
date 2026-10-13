// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Interaction/HorrorDoor.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

AHorrorDoor::AHorrorDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetupAttachment(RootComponent);
	DoorFrameMesh->SetCollisionProfileName(TEXT("BlockAll"));

	DoorHinge = CreateDefaultSubobject<USceneComponent>(TEXT("DoorHinge"));
	DoorHinge->SetupAttachment(RootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorHinge);
	DoorMesh->SetCollisionProfileName(TEXT("BlockAll"));

	// Assign default door mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultDoorAsset(TEXT("/Game/MafiaBarnPack/Meshes/Architecture/SM_Barn_HayDoor_Leaf_L.SM_Barn_HayDoor_Leaf_L"));
	if (DefaultDoorAsset.Succeeded() && DoorMesh)
	{
		DoorMesh->SetStaticMesh(DefaultDoorAsset.Object);
	}
}

void AHorrorDoor::BeginPlay()
{
	Super::BeginPlay();

	// Fallback to default door if not set
	if (DoorMesh && !DoorMesh->GetStaticMesh())
	{
		UStaticMesh* DefaultDoor = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Architecture/SM_Barn_HayDoor_Leaf_L.SM_Barn_HayDoor_Leaf_L"));
		if (DefaultDoor)
		{
			DoorMesh->SetStaticMesh(DefaultDoor);
		}
	}

	CurrentYaw = bIsOpen ? OpenAngle : 0.0f;
	TargetYaw = CurrentYaw;
	DoorHinge->SetRelativeRotation(FRotator(0.0f, CurrentYaw, 0.0f));
}

void AHorrorDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, MovementSpeed);
		DoorHinge->SetRelativeRotation(FRotator(0.0f, CurrentYaw, 0.0f));

		if (FMath::IsNearlyEqual(CurrentYaw, TargetYaw, 0.5f))
		{
			CurrentYaw = TargetYaw;
			DoorHinge->SetRelativeRotation(FRotator(0.0f, CurrentYaw, 0.0f));
			bIsMoving = false;
		}
	}
}

bool AHorrorDoor::CanInteract_Implementation(AHorrorCharacter* Interactor)
{
	return Interactor != nullptr;
}

FText AHorrorDoor::GetInteractionPrompt_Implementation(AHorrorCharacter* Interactor)
{
	if (bIsLocked)
	{
		if (Interactor)
		{
			if (UHorrorInventoryComponent* Inv = Interactor->FindComponentByClass<UHorrorInventoryComponent>())
			{
				if (!RequiredKeyId.IsNone() && Inv->HasItem(RequiredKeyId))
				{
					return FText::FromString(TEXT("Unlock Door"));
				}
			}
		}
		return FText::FromString(TEXT("Locked"));
	}

	return bIsOpen ? FText::FromString(TEXT("Close Door")) : FText::FromString(TEXT("Open Door"));
}

void AHorrorDoor::OnFocusGained_Implementation(AHorrorCharacter* Interactor)
{
	if (DoorMesh)
	{
		DoorMesh->SetRenderCustomDepth(true);
	}
}

void AHorrorDoor::OnFocusLost_Implementation(AHorrorCharacter* Interactor)
{
	if (DoorMesh)
	{
		DoorMesh->SetRenderCustomDepth(false);
	}
}

void AHorrorDoor::Interact_Implementation(AHorrorCharacter* Interactor)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (bIsLocked)
	{
		// Check if interactor holds the key
		if (Interactor)
		{
			if (UHorrorInventoryComponent* Inv = Interactor->FindComponentByClass<UHorrorInventoryComponent>())
			{
				if (!RequiredKeyId.IsNone() && Inv->HasItem(RequiredKeyId))
				{
					// Unlock successfully
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(4001, 3.0f, FColor::Green, FString::Printf(TEXT("Unlocked with %s!"), *RequiredKeyId.ToString()));
					}
					UnlockDoor();
					OpenDoor();
					return;
				}
			}
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(4002, 2.5f, FColor::Red, FString::Printf(TEXT("The door is locked! Requires: %s"), *RequiredKeyId.ToString()));
		}

		// Locked rattle sound
		if (LockedRattleSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, LockedRattleSound, GetActorLocation());
		}
		return;
	}

	// Toggle open / close
	ToggleDoor();
}

void AHorrorDoor::OpenDoor()
{
	TargetYaw = OpenAngle;
	bIsOpen = true;
	bIsMoving = true;

	if (CreakOpenSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CreakOpenSound, GetActorLocation());
	}

	OnDoorStateChanged.Broadcast(true);
}

void AHorrorDoor::CloseDoor()
{
	TargetYaw = 0.0f;
	bIsOpen = false;
	bIsMoving = true;

	if (CreakCloseSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CreakCloseSound, GetActorLocation());
	}

	OnDoorStateChanged.Broadcast(false);
}

void AHorrorDoor::ToggleDoor()
{
	if (bIsMoving)
	{
		return;
	}

	if (bIsOpen)
	{
		CloseDoor();
	}
	else
	{
		OpenDoor();
	}
}

void AHorrorDoor::SlamShut(bool bLockAfterSlam)
{
	TargetYaw = 0.0f;
	CurrentYaw = 0.0f;
	bIsOpen = false;
	bIsMoving = false;
	DoorHinge->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	if (bLockAfterSlam)
	{
		bIsLocked = true;
	}

	if (SlamBangSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlamBangSound, GetActorLocation());
	}

	OnDoorStateChanged.Broadcast(false);
	OnDoorSlammed.Broadcast();
}

void AHorrorDoor::UnlockDoor()
{
	bIsLocked = false;

	if (UnlockSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UnlockSound, GetActorLocation());
	}
}

void AHorrorDoor::LockDoor(FName WithKey)
{
	bIsLocked = true;
	if (!WithKey.IsNone())
	{
		RequiredKeyId = WithKey;
	}
}
