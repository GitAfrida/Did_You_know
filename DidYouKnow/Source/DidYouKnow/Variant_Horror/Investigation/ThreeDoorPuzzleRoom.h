// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThreeDoorPuzzleRoom.generated.h"

class UBoxComponent;
class AHorrorDoor;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleDeathSequenceStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleSolved);

/**
 * Manages the deadly 3-door riddle chamber in Graves Country House.
 * Mila's Clue: "Did you know that people usually choose the middle option when they're uncertain?"
 * The middle door is a lethal trap triggering a jumpscare and the "YOU DIED - RETRY" loop.
 */
UCLASS()
class DIDYOUKNOW_API AThreeDoorPuzzleRoom : public AActor
{
	GENERATED_BODY()

public:
	AThreeDoorPuzzleRoom();

	UFUNCTION(BlueprintCallable, Category = "Horror|Puzzle")
	void ResetPuzzleRoom();

	/** Triggers the death jumpscare and death screen sequence */
	UFUNCTION(BlueprintCallable, Category = "Horror|Puzzle")
	void TriggerDeathTrap();

	UPROPERTY(BlueprintAssignable, Category = "Horror|Puzzle")
	FOnPuzzleDeathSequenceStarted OnPuzzleDeathSequenceStarted;

	UPROPERTY(BlueprintAssignable, Category = "Horror|Puzzle")
	FOnPuzzleSolved OnPuzzleSolved;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEntryOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDoorStateChanged(bool bIsOpen);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> EntryTrigger;

	/** Room entrance door that slams shut behind Cross */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Doors")
	TObjectPtr<AHorrorDoor> EntryDoor;

	/** Left door */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Doors")
	TObjectPtr<AHorrorDoor> LeftDoor;

	/** Middle door (Default lethal trap door) */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Doors")
	TObjectPtr<AHorrorDoor> MiddleDoor;

	/** Right door */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Doors")
	TObjectPtr<AHorrorDoor> RightDoor;

	/** Index of the trap door: 0 = Left, 1 = Middle (Default), 2 = Right */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Puzzle")
	int32 TrapDoorIndex = 1;

	/** Checkpoint transform to place Cross upon clicking Retry */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Puzzle")
	FTransform CheckpointTransform;

	/** Audio played when triggering death trap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Audio")
	TObjectPtr<USoundBase> TrapScreechAudio;

private:
	bool bRoomEntered = false;
	bool bPuzzleCompleted = false;
};
