// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/MilaDialogueSubsystem.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UMilaDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMilaDialogueSubsystem::TriggerMilaFact(FName FactId, const FText& FactQuote, const FText& ClueDeduction, USoundBase* VoiceAudio, float DisplayDuration)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Play voice audio if provided
	if (VoiceAudio)
	{
		UGameplayStatics::PlaySound2D(World, VoiceAudio);
	}

	// Broadcast for UI horror subtitle rendering
	OnMilaFactDialogue.Broadcast(FactId, FactQuote, ClueDeduction, DisplayDuration);

	// Sync fact into Cross's detective notebook automatically
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (PlayerPawn)
	{
		if (UHorrorInventoryComponent* InventoryComp = PlayerPawn->FindComponentByClass<UHorrorInventoryComponent>())
		{
			InventoryComp->RecordMilaFact(FactId, FactQuote, ClueDeduction);
		}
	}
}

void UMilaDialogueSubsystem::TriggerDirectDialogue(const FText& Speaker, const FText& DialogueText, USoundBase* VoiceAudio, float DisplayDuration)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (VoiceAudio)
	{
		UGameplayStatics::PlaySound2D(World, VoiceAudio);
	}

	OnDirectDialogue.Broadcast(Speaker, DialogueText, DisplayDuration);
}
