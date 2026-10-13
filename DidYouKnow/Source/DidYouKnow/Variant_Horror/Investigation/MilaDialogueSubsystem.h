// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Variant_Horror/Investigation/HorrorTypes.h"
#include "MilaDialogueSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMilaFactDialogue, FName, FactId, const FText&, FactQuote, const FText&, ClueDeduction, float, DisplayDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDirectDialogue, const FText&, Speaker, const FText&, DialogueText, float, DisplayDuration);

/**
 * World subsystem responsible for triggering Mila's eerie voice cues ("Facts are clues"),
 * subtitle delivery, and syncing clues directly into Cross Grover's detective notebook.
 */
UCLASS()
class DIDYOUKNOW_API UMilaDialogueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Triggers a Mila Fact cue (e.g., "Did you know octopuses have three hearts?").
	 * Plays audio, presents stylized horror subtitles, and records the clue into Cross's notebook.
	 */
	UFUNCTION(BlueprintCallable, Category = "Horror|MilaDialogue")
	void TriggerMilaFact(FName FactId, const FText& FactQuote, const FText& ClueDeduction = FText::GetEmpty(), USoundBase* VoiceAudio = nullptr, float DisplayDuration = 5.0f);

	/**
	 * Triggers direct narrative dialogue (e.g. Cross: "Who's there?", Mila: "That's not a very useful question.")
	 */
	UFUNCTION(BlueprintCallable, Category = "Horror|MilaDialogue")
	void TriggerDirectDialogue(const FText& Speaker, const FText& DialogueText, USoundBase* VoiceAudio = nullptr, float DisplayDuration = 4.0f);

	/** Delegate fired whenever Mila drops a fact */
	UPROPERTY(BlueprintAssignable, Category = "Horror|MilaDialogue")
	FOnMilaFactDialogue OnMilaFactDialogue;

	/** Delegate fired for direct conversation between Cross and Mila */
	UPROPERTY(BlueprintAssignable, Category = "Horror|MilaDialogue")
	FOnDirectDialogue OnDirectDialogue;
};
