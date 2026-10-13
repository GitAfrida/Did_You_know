// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Variant_Horror/Interaction/HorrorInteractableInterface.h"
#include "HorrorInteractionComponent.generated.h"

class AHorrorCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableFocused, AActor*, InteractableActor, const FText&, PromptText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableUnfocused);

/**
 * Handles Cross Grover's first-person raycasting, interaction prompts, and interact execution.
 */
UCLASS(ClassGroup = (Horror), meta = (BlueprintSpawnableComponent))
class DIDYOUKNOW_API UHorrorInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHorrorInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Attempts to interact with the currently focused interactable object */
	UFUNCTION(BlueprintCallable, Category = "Horror|Interaction")
	bool TryInteract();

	/** Gets the prompt text of the currently focused interactable */
	UFUNCTION(BlueprintPure, Category = "Horror|Interaction")
	FText GetCurrentPromptText() const { return CurrentPrompt; }

	/** Gets the currently focused actor */
	UFUNCTION(BlueprintPure, Category = "Horror|Interaction")
	AActor* GetFocusedActor() const { return CurrentFocusedActor.Get(); }

	/** Broadcast when Cross looks at an interactable object */
	UPROPERTY(BlueprintAssignable, Category = "Horror|Interaction")
	FOnInteractableFocused OnInteractableFocused;

	/** Broadcast when Cross looks away from an interactable object */
	UPROPERTY(BlueprintAssignable, Category = "Horror|Interaction")
	FOnInteractableUnfocused OnInteractableUnfocused;

protected:
	virtual void BeginPlay() override;

	/** Executes line trace from the first person camera */
	void PerformTrace();

	/** Clears current focus and fires unfocus events */
	void ClearFocus();

protected:
	/** Maximum distance Cross can reach to interact with items (in cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Interaction", meta = (ClampMin = "50.0", ClampMax = "500.0"))
	float InteractionDistance = 350.0f;

	/** Collision channel used to detect interactable items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

private:
	/** Cached owner character */
	TWeakObjectPtr<AHorrorCharacter> OwnerCharacter;

	/** Current interactable interface under Cross's crosshair */
	TScriptInterface<IHorrorInteractable> CurrentInteractable;

	/** Current actor under Cross's crosshair */
	TWeakObjectPtr<AActor> CurrentFocusedActor;

	/** Current prompt string */
	FText CurrentPrompt;
};
