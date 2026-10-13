// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/HorrorObjectiveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UHorrorObjectiveSubsystem::SetObjective(const FText& NewObjective, USoundBase* ObjectiveSound)
{
	if (NewObjective.EqualTo(CurrentObjective))
	{
		return;
	}

	CurrentObjective = NewObjective;

	if (ObjectiveSound && GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ObjectiveSound);
	}

	OnObjectiveChanged.Broadcast(CurrentObjective);
}

void UHorrorObjectiveSubsystem::CompleteObjective(const FText& ObjectiveToComplete)
{
	CompletedObjectives.Add(ObjectiveToComplete);
	OnObjectiveCompleted.Broadcast(ObjectiveToComplete);
}
