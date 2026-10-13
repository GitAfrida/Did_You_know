// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HorrorObjectiveSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveChanged, const FText&, NewObjective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, const FText&, CompletedObjective);

/**
 * World subsystem for tracking and displaying current investigation objectives on Cross's HUD.
 */
UCLASS()
class DIDYOUKNOW_API UHorrorObjectiveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Sets a new primary investigation objective (e.g. "Find the study mentioned in the property records") */
	UFUNCTION(BlueprintCallable, Category = "Horror|Objectives")
	void SetObjective(const FText& NewObjective, USoundBase* ObjectiveSound = nullptr);

	/** Marks an objective completed */
	UFUNCTION(BlueprintCallable, Category = "Horror|Objectives")
	void CompleteObjective(const FText& ObjectiveToComplete);

	/** Gets the active objective */
	UFUNCTION(BlueprintPure, Category = "Horror|Objectives")
	FText GetCurrentObjective() const { return CurrentObjective; }

	/** Broadcast when the active objective changes */
	UPROPERTY(BlueprintAssignable, Category = "Horror|Objectives")
	FOnObjectiveChanged OnObjectiveChanged;

	/** Broadcast when an objective is finished */
	UPROPERTY(BlueprintAssignable, Category = "Horror|Objectives")
	FOnObjectiveCompleted OnObjectiveCompleted;

private:
	/** Currently active objective displayed on screen */
	UPROPERTY(VisibleAnywhere, Category = "Horror|Objectives")
	FText CurrentObjective;

	/** History of completed objectives */
	UPROPERTY(VisibleAnywhere, Category = "Horror|Objectives")
	TArray<FText> CompletedObjectives;
};
