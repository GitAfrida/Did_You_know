// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/ThreeDoorPuzzleRoom.h"
#include "Variant_Horror/Interaction/HorrorDoor.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Investigation/MilaDialogueSubsystem.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AThreeDoorPuzzleRoom::AThreeDoorPuzzleRoom()
{
	PrimaryActorTick.bCanEverTick = false;

	EntryTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EntryTrigger"));
	RootComponent = EntryTrigger;
	EntryTrigger->SetCollisionProfileName(TEXT("Trigger"));
	EntryTrigger->SetBoxExtent(FVector(300.0f, 300.0f, 150.0f));
}

void AThreeDoorPuzzleRoom::ConfigureDoors(AHorrorDoor* InLeft, AHorrorDoor* InMiddle, AHorrorDoor* InRight, AHorrorDoor* InEntry)
{
	LeftDoor = InLeft;
	MiddleDoor = InMiddle;
	RightDoor = InRight;
	EntryDoor = InEntry;

	if (MiddleDoor)
	{
		MiddleDoor->OnDoorStateChanged.RemoveDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
		MiddleDoor->OnDoorStateChanged.AddDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
	}
	if (LeftDoor)
	{
		LeftDoor->OnDoorStateChanged.RemoveDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
		LeftDoor->OnDoorStateChanged.AddDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
	}
	if (RightDoor)
	{
		RightDoor->OnDoorStateChanged.RemoveDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
		RightDoor->OnDoorStateChanged.AddDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
	}
}

void AThreeDoorPuzzleRoom::BeginPlay()
{
	Super::BeginPlay();

	EntryTrigger->OnComponentBeginOverlap.AddDynamic(this, &AThreeDoorPuzzleRoom::OnEntryOverlap);

	// Setup door listeners for doors assigned via editor details
	if (MiddleDoor)
	{
		MiddleDoor->OnDoorStateChanged.RemoveDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
		MiddleDoor->OnDoorStateChanged.AddDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
	}
	if (LeftDoor)
	{
		LeftDoor->OnDoorStateChanged.RemoveDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
		LeftDoor->OnDoorStateChanged.AddDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
	}
	if (RightDoor)
	{
		RightDoor->OnDoorStateChanged.RemoveDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
		RightDoor->OnDoorStateChanged.AddDynamic(this, &AThreeDoorPuzzleRoom::OnDoorStateChanged);
	}
}

void AThreeDoorPuzzleRoom::OnEntryOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bRoomEntered)
	{
		return;
	}

	if (AHorrorCharacter* Character = Cast<AHorrorCharacter>(OtherActor))
	{
		bRoomEntered = true;

		// 1. Slam the room entrance door shut behind Cross
		if (EntryDoor)
		{
			EntryDoor->SlamShut(true);
		}

		// 2. Deliver Mila's crucial survival clue
		if (UWorld* World = GetWorld())
		{
			if (UMilaDialogueSubsystem* DialogueSubsystem = World->GetSubsystem<UMilaDialogueSubsystem>())
			{
				DialogueSubsystem->TriggerMilaFact(
					FName("Fact_MiddleOption"),
					FText::FromString(TEXT("Did you know that people usually choose the middle option when they're uncertain?")),
					FText::FromString(TEXT("Avoid the middle door—it is a lethal trap."))
				);
			}
		}
	}
}

void AThreeDoorPuzzleRoom::OnDoorStateChanged(bool bIsOpen)
{
	if (!bIsOpen || bPuzzleCompleted)
	{
		return;
	}

	// Check which door opened
	AHorrorDoor* TrapDoor = (TrapDoorIndex == 1) ? MiddleDoor : (TrapDoorIndex == 0 ? LeftDoor : RightDoor);

	if (TrapDoor && TrapDoor->IsOpen())
	{
		TriggerDeathTrap();
	}
	else
	{
		// Safe door opened!
		bPuzzleCompleted = true;

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			if (UHorrorInventoryComponent* Inv = PlayerPawn->FindComponentByClass<UHorrorInventoryComponent>())
			{
				Inv->MarkFactSolved(FName("Fact_MiddleOption"));
			}
		}

		OnPuzzleSolved.Broadcast();
	}
}

void AThreeDoorPuzzleRoom::TriggerDeathTrap()
{
	if (TrapScreechAudio && GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), TrapScreechAudio);
	}

	OnPuzzleDeathSequenceStarted.Broadcast();
}

void AThreeDoorPuzzleRoom::ResetPuzzleRoom()
{
	// Close and reset doors
	if (LeftDoor) LeftDoor->CloseDoor();
	if (MiddleDoor) MiddleDoor->CloseDoor();
	if (RightDoor) RightDoor->CloseDoor();

	// Respawn Cross at checkpoint
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn && !CheckpointTransform.GetLocation().IsZero())
	{
		PlayerPawn->SetActorTransform(CheckpointTransform, false, nullptr, ETeleportType::ResetPhysics);
	}
}
