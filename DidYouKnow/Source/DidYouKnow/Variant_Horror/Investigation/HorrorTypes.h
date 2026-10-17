// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HorrorTypes.generated.h"

class UTexture2D;

/**
 * Represents a Mila Fact recorded in Cross Grover's investigation notebook.
 * In "Did You Know?", facts are not trivia—they are clues and survival rules.
 */
USTRUCT(BlueprintType)
struct FMilaFactEntry
{
	GENERATED_BODY()

	/** Unique identifier for the fact (e.g., Fact_Octopus, Fact_MiddleOption, Fact_StoppedClock) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Facts")
	FName FactId = NAME_None;

	/** The verbatim quote from Mila (e.g. "Did you know octopuses have three hearts?") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Facts")
	FText FactQuote;

	/** Cross's notebook deduction or explanation of how the fact connects to the case */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Facts")
	FText NotebookDeduction;

	/** Whether this fact has been applied to solve a puzzle or avoid a hazard */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Facts")
	bool bIsSolved = false;

	/** Timestamp or order when the fact was heard */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Investigation|Facts")
	FDateTime TimeDiscovered = FDateTime::MinValue();

	FMilaFactEntry()
		: TimeDiscovered(FDateTime::UtcNow())
	{}

	FMilaFactEntry(FName InId, const FText& InQuote, const FText& InDeduction = FText::GetEmpty())
		: FactId(InId)
		, FactQuote(InQuote)
		, NotebookDeduction(InDeduction)
		, bIsSolved(false)
		, TimeDiscovered(FDateTime::UtcNow())
	{}
};

/**
 * Represents a document, photograph, or record collected or inspected in Graves Country House.
 */
USTRUCT(BlueprintType)
struct FInvestigationDocument
{
	GENERATED_BODY()

	/** Unique ID for the document (e.g. Doc_VictorPortrait, Doc_MaraNote, Doc_AutopsyReport) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document")
	FName DocumentId = NAME_None;

	/** Title displayed in the notebook and inspection HUD */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document")
	FText Title;

	/** Full transcript or examination text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document")
	FText Content;

	/** Optional graphic/texture for visual inspection */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document")
	TObjectPtr<UTexture2D> DocumentTexture = nullptr;

	/** Has this document been collected and placed in the notebook? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document")
	bool bIsCollected = false;

	/** If true, this item (such as a photograph) has a back side that can be flipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document")
	bool bHasReverseSide = false;

	/** Text or handwritten note on the back side (e.g. "He never noticed the second key") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document", meta = (EditCondition = "bHasReverseSide"))
	FText ReverseSideContent;

	/** Optional graphic/texture for the reverse side */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigation|Document", meta = (EditCondition = "bHasReverseSide"))
	TObjectPtr<UTexture2D> ReverseSideTexture = nullptr;
};

/**
 * States for interactive doors throughout the mansion.
 */
UENUM(BlueprintType)
enum class EHorrorDoorState : uint8
{
	Closed UMETA(DisplayName = "Closed"),
	Opening UMETA(DisplayName = "Opening"),
	Open UMETA(DisplayName = "Open"),
	Closing UMETA(DisplayName = "Closing"),
	Locked UMETA(DisplayName = "Locked")
};
