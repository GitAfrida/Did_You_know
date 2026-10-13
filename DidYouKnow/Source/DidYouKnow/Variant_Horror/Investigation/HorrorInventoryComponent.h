// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Variant_Horror/Investigation/HorrorTypes.h"
#include "HorrorInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemAdded, FName, ItemId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDocumentCollected, const FInvestigationDocument&, Document);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMilaFactRecorded, const FMilaFactEntry&, Fact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFactSolved, FName, FactId);

/**
 * Manages Cross Grover's inventory (keys, physical evidence) and Detective Notebook (Mila facts, documents).
 */
UCLASS(ClassGroup = (Horror), meta = (BlueprintSpawnableComponent))
class DIDYOUKNOW_API UHorrorInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHorrorInventoryComponent();

	/** Adds an item/key to Cross's inventory */
	UFUNCTION(BlueprintCallable, Category = "Horror|Inventory")
	void AddItem(FName ItemId);

	/** Checks if Cross possesses a specific key or item */
	UFUNCTION(BlueprintPure, Category = "Horror|Inventory")
	bool HasItem(FName ItemId) const;

	/** Removes an item from inventory (e.g. used single-use key) */
	UFUNCTION(BlueprintCallable, Category = "Horror|Inventory")
	bool RemoveItem(FName ItemId);

	/** Adds an examined document or photograph to the investigation notebook */
	UFUNCTION(BlueprintCallable, Category = "Horror|Notebook")
	void AddDocument(const FInvestigationDocument& Document);

	/** Records a new fact uttered by Mila into Cross's detective notebook */
	UFUNCTION(BlueprintCallable, Category = "Horror|Notebook")
	void RecordMilaFact(FName FactId, const FText& FactQuote, const FText& Deduction = FText::GetEmpty());

	/** Marks a fact as resolved/applied to a puzzle */
	UFUNCTION(BlueprintCallable, Category = "Horror|Notebook")
	void MarkFactSolved(FName FactId);

	/** Gets all recorded Mila facts */
	UFUNCTION(BlueprintPure, Category = "Horror|Notebook")
	const TArray<FMilaFactEntry>& GetRecordedFacts() const { return RecordedFacts; }

	/** Gets all collected documents */
	UFUNCTION(BlueprintPure, Category = "Horror|Notebook")
	const TArray<FInvestigationDocument>& GetCollectedDocuments() const { return CollectedDocuments; }

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Horror|Inventory")
	FOnInventoryItemAdded OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Horror|Notebook")
	FOnDocumentCollected OnDocumentCollected;

	UPROPERTY(BlueprintAssignable, Category = "Horror|Notebook")
	FOnMilaFactRecorded OnMilaFactRecorded;

	UPROPERTY(BlueprintAssignable, Category = "Horror|Notebook")
	FOnFactSolved OnFactSolved;

protected:
	/** Inventory items (keys, cassette, etc.) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Inventory")
	TSet<FName> InventoryItems;

	/** Documents and photographs logged in Cross's notebook */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Notebook")
	TArray<FInvestigationDocument> CollectedDocuments;

	/** Mila's facts gathered during the investigation */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Notebook")
	TArray<FMilaFactEntry> RecordedFacts;
};
