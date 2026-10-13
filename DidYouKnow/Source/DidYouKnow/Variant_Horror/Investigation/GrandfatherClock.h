// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Variant_Horror/Interaction/HorrorInteractableInterface.h"
#include "Variant_Horror/Investigation/HorrorTypes.h"
#include "GrandfatherClock.generated.h"

class UStaticMeshComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClockSecretRevealed);

/**
 * Interactive stopped grandfather clock in Graves Country House.
 * Mila's Clue: "A stopped clock is correct twice a day"
 * Inspecting it reveals a secret document hidden behind the mechanism.
 */
UCLASS()
class DIDYOUKNOW_API AGrandfatherClock : public AActor, public IHorrorInteractable
{
	GENERATED_BODY()

public:
	AGrandfatherClock();

	// IHorrorInteractable Interface
	virtual bool CanInteract_Implementation(AHorrorCharacter* Interactor) override;
	virtual void Interact_Implementation(AHorrorCharacter* Interactor) override;
	virtual FText GetInteractionPrompt_Implementation(AHorrorCharacter* Interactor) override;
	virtual void OnFocusGained_Implementation(AHorrorCharacter* Interactor) override;
	virtual void OnFocusLost_Implementation(AHorrorCharacter* Interactor) override;

	UPROPERTY(BlueprintAssignable, Category = "Horror|Clock")
	FOnClockSecretRevealed OnClockSecretRevealed;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ClockMesh;

	/** Document hidden behind the stopped grandfather clock */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Clock")
	FInvestigationDocument HiddenDocument;

	/** Audio played when opening the secret clock compartment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Clock")
	TObjectPtr<USoundBase> SecretCompartmentAudio;

	/** Has the secret document already been discovered? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Clock")
	bool bSecretRevealed = false;
};
