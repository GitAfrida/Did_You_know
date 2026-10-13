// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"

UHorrorInventoryComponent::UHorrorInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHorrorInventoryComponent::AddItem(FName ItemId)
{
	if (ItemId.IsNone())
	{
		return;
	}

	InventoryItems.Add(ItemId);
	OnInventoryItemAdded.Broadcast(ItemId);
}

bool UHorrorInventoryComponent::HasItem(FName ItemId) const
{
	return InventoryItems.Contains(ItemId);
}

bool UHorrorInventoryComponent::RemoveItem(FName ItemId)
{
	return InventoryItems.Remove(ItemId) > 0;
}

void UHorrorInventoryComponent::AddDocument(const FInvestigationDocument& Document)
{
	for (const FInvestigationDocument& ExistingDoc : CollectedDocuments)
	{
		if (ExistingDoc.DocumentId == Document.DocumentId)
		{
			return; // Already present in notebook
		}
	}

	CollectedDocuments.Add(Document);
	OnDocumentCollected.Broadcast(Document);
}

void UHorrorInventoryComponent::RecordMilaFact(FName FactId, const FText& FactQuote, const FText& Deduction)
{
	for (FMilaFactEntry& ExistingFact : RecordedFacts)
	{
		if (ExistingFact.FactId == FactId)
		{
			// If already recorded, update deduction if new
			if (!Deduction.IsEmpty() && ExistingFact.NotebookDeduction.IsEmpty())
			{
				ExistingFact.NotebookDeduction = Deduction;
			}
			return;
		}
	}

	FMilaFactEntry NewFact(FactId, FactQuote, Deduction);
	RecordedFacts.Add(NewFact);
	OnMilaFactRecorded.Broadcast(NewFact);
}

void UHorrorInventoryComponent::MarkFactSolved(FName FactId)
{
	for (FMilaFactEntry& Fact : RecordedFacts)
	{
		if (Fact.FactId == FactId)
		{
			Fact.bIsSolved = true;
			OnFactSolved.Broadcast(FactId);
			break;
		}
	}
}
