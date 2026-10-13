// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HorrorUI.generated.h"

class AHorrorCharacter;

/**
 * Main Horror HUD widget for "Did You Know?".
 * Displays sprint meter, interaction prompt, Mila facts ("Facts are clues"),
 * dialogue subtitles, and active investigation objectives.
 */
UCLASS(abstract)
class DIDYOUKNOW_API UHorrorUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Sets up delegate listeners for the passed character and subsystems */
	virtual void SetupCharacter(AHorrorCharacter* HorrorCharacter);

	/** Sprint meter listeners */
	UFUNCTION()
	void OnSprintMeterUpdated(float Percent);

	UFUNCTION()
	void OnSprintStateChanged(bool bSprinting);

	/** Interaction prompt listeners */
	UFUNCTION()
	void OnInteractableFocused(AActor* FocusedActor, const FText& Prompt);

	UFUNCTION()
	void OnInteractableUnfocused();

	/** Subtitle & Mila fact listeners */
	UFUNCTION()
	void OnMilaFactTriggered(FName FactId, const FText& FactQuote, const FText& ClueDeduction, float Duration);

	UFUNCTION()
	void OnDirectDialogueTriggered(const FText& Speaker, const FText& DialogueText, float Duration);

	/** Objective listener */
	UFUNCTION()
	void OnObjectiveChanged(const FText& NewObjective);

	/** Notebook toggle listener */
	UFUNCTION()
	void OnNotebookToggleRequested();

protected:
	/** Blueprint events for UI styling and animations */
	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Sprint Meter Updated"))
	void BP_SprintMeterUpdated(float Percent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Sprint State Changed"))
	void BP_SprintStateChanged(bool bSprinting);

	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Interaction Prompt Updated"))
	void BP_InteractionPromptUpdated(bool bHasPrompt, const FText& PromptText);

	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Mila Fact Displayed"))
	void BP_MilaFactDisplayed(FName FactId, const FText& FactQuote, const FText& ClueDeduction, float Duration);

	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Dialogue Displayed"))
	void BP_DialogueDisplayed(const FText& Speaker, const FText& DialogueText, float Duration);

	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Objective Updated"))
	void BP_ObjectiveUpdated(const FText& ObjectiveText);

	UFUNCTION(BlueprintImplementableEvent, Category = "Horror|UI", meta = (DisplayName = "Toggle Notebook"))
	void BP_ToggleNotebook();
};
