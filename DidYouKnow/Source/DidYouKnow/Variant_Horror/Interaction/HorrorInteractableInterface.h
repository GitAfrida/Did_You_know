// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HorrorInteractableInterface.generated.h"

class AHorrorCharacter;

UINTERFACE(BlueprintType)
class DIDYOUKNOW_API UHorrorInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface implemented by any object or actor in Graves Country House that Cross can interact with.
 */
class DIDYOUKNOW_API IHorrorInteractable
{
	GENERATED_BODY()

public:
	/** Returns whether Cross can currently interact with this object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AHorrorCharacter* Interactor);

	/** Executes the primary interaction (e.g. open door, read note, pick up key) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AHorrorCharacter* Interactor);

	/** Returns prompt text shown on the player's HUD (e.g. "Examine Portrait", "Open Door", "Take Key") */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt(AHorrorCharacter* Interactor);

	/** Called when Cross centers his gaze on the interactable */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnFocusGained(AHorrorCharacter* Interactor);

	/** Called when Cross looks away from the interactable */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnFocusLost(AHorrorCharacter* Interactor);
};
