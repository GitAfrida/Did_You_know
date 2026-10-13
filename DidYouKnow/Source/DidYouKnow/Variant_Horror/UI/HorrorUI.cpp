// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/UI/HorrorUI.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Interaction/HorrorInteractionComponent.h"
#include "Variant_Horror/Investigation/MilaDialogueSubsystem.h"
#include "Variant_Horror/Investigation/HorrorObjectiveSubsystem.h"
#include "Engine/World.h"

void UHorrorUI::SetupCharacter(AHorrorCharacter* HorrorCharacter)
{
	if (!HorrorCharacter)
	{
		return;
	}

	// 1. Sprint meter bindings
	HorrorCharacter->OnSprintMeterUpdated.AddDynamic(this, &UHorrorUI::OnSprintMeterUpdated);
	HorrorCharacter->OnSprintStateChanged.AddDynamic(this, &UHorrorUI::OnSprintStateChanged);

	// 2. Interaction component bindings
	if (UHorrorInteractionComponent* InterComp = HorrorCharacter->GetInteractionComponent())
	{
		InterComp->OnInteractableFocused.AddDynamic(this, &UHorrorUI::OnInteractableFocused);
		InterComp->OnInteractableUnfocused.AddDynamic(this, &UHorrorUI::OnInteractableUnfocused);
	}

	// 3. Notebook toggle binding
	HorrorCharacter->OnNotebookToggleRequested.AddDynamic(this, &UHorrorUI::OnNotebookToggleRequested);

	// 4. World subsystem bindings
	if (UWorld* World = GetWorld())
	{
		if (UMilaDialogueSubsystem* DialogueSubsystem = World->GetSubsystem<UMilaDialogueSubsystem>())
		{
			DialogueSubsystem->OnMilaFactDialogue.AddDynamic(this, &UHorrorUI::OnMilaFactTriggered);
			DialogueSubsystem->OnDirectDialogue.AddDynamic(this, &UHorrorUI::OnDirectDialogueTriggered);
		}

		if (UHorrorObjectiveSubsystem* ObjSubsystem = World->GetSubsystem<UHorrorObjectiveSubsystem>())
		{
			ObjSubsystem->OnObjectiveChanged.AddDynamic(this, &UHorrorUI::OnObjectiveChanged);
			
			// Initialize with current objective if already set
			const FText CurrentObj = ObjSubsystem->GetCurrentObjective();
			if (!CurrentObj.IsEmpty())
			{
				BP_ObjectiveUpdated(CurrentObj);
			}
		}
	}
}

void UHorrorUI::OnSprintMeterUpdated(float Percent)
{
	BP_SprintMeterUpdated(Percent);
}

void UHorrorUI::OnSprintStateChanged(bool bSprinting)
{
	BP_SprintStateChanged(bSprinting);
}

void UHorrorUI::OnInteractableFocused(AActor* FocusedActor, const FText& Prompt)
{
	BP_InteractionPromptUpdated(true, Prompt);
}

void UHorrorUI::OnInteractableUnfocused()
{
	BP_InteractionPromptUpdated(false, FText::GetEmpty());
}

void UHorrorUI::OnMilaFactTriggered(FName FactId, const FText& FactQuote, const FText& ClueDeduction, float Duration)
{
	BP_MilaFactDisplayed(FactId, FactQuote, ClueDeduction, Duration);
}

void UHorrorUI::OnDirectDialogueTriggered(const FText& Speaker, const FText& DialogueText, float Duration)
{
	BP_DialogueDisplayed(Speaker, DialogueText, Duration);
}

void UHorrorUI::OnObjectiveChanged(const FText& NewObjective)
{
	BP_ObjectiveUpdated(NewObjective);
}

void UHorrorUI::OnNotebookToggleRequested()
{
	BP_ToggleNotebook();
}
