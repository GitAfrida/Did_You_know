// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Variant_Horror/Interaction/HorrorInteractableInterface.h"
#include "Variant_Horror/Investigation/HorrorTypes.h"
#include "HorrorInspectable.generated.h"

class UStaticMeshComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInspectableInspected, const FInvestigationDocument&, Document);

/**
 * Base actor for inspectable evidence, documents, portraits, and clues across Graves Country House.
 */
UCLASS()
class DIDYOUKNOW_API AHorrorInspectable : public AActor, public IHorrorInteractable
{
	GENERATED_BODY()

public:
	AHorrorInspectable();

	// IHorrorInteractable Interface
	virtual bool CanInteract_Implementation(AHorrorCharacter* Interactor) override;
	virtual void Interact_Implementation(AHorrorCharacter* Interactor) override;
	virtual FText GetInteractionPrompt_Implementation(AHorrorCharacter* Interactor) override;
	virtual void OnFocusGained_Implementation(AHorrorCharacter* Interactor) override;
	virtual void OnFocusLost_Implementation(AHorrorCharacter* Interactor) override;

	/** Gets the document data associated with this inspectable */
	UFUNCTION(BlueprintPure, Category = "Horror|Inspection")
	const FInvestigationDocument& GetDocumentData() const { return DocumentData; }

	UPROPERTY(BlueprintAssignable, Category = "Horror|Inspection")
	FOnInspectableInspected OnInspectableInspected;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	/** Examination details (Title, full text, texture) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection")
	FInvestigationDocument DocumentData;

	/** Custom prompt text displayed to Cross (e.g. "Examine Portrait", "Read Note", "Take Key") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection")
	FText CustomPrompt;

	/** If true, Cross collects this item into his inventory and the actor is hidden/destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection")
	bool bIsPickup = false;

	/** Item ID added to Cross's key items if bIsPickup is true */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection", meta = (EditCondition = "bIsPickup"))
	FName PickupItemId = NAME_None;

	/** Sound effect played upon examination */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection")
	TObjectPtr<USoundBase> InspectSound;

	/** Optional Mila Fact triggered upon examining this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection|Mila")
	FName TriggerMilaFactId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection|Mila")
	FText TriggerMilaFactQuote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection|Mila")
	FText TriggerMilaFactDeduction;

	/** Optional new objective triggered upon examining this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Inspection|Objective")
	FText NewObjectiveOnInspect;

	/** Has this object already been examined? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Inspection")
	bool bHasBeenExamined = false;
};
