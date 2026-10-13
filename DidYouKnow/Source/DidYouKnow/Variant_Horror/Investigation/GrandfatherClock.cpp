// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/GrandfatherClock.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AGrandfatherClock::AGrandfatherClock()
{
	PrimaryActorTick.bCanEverTick = false;

	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
	RootComponent = ClockMesh;
	ClockMesh->SetCollisionProfileName(TEXT("BlockAll"));

	// Initialize default document data
	HiddenDocument.DocumentId = FName("Doc_ClockSecret");
	HiddenDocument.Title = FText::FromString(TEXT("Document Hidden in Grandfather Clock"));
	HiddenDocument.Content = FText::FromString(TEXT("A hidden estate deed and transaction ledger connecting Victor Graves to illicit shipments."));
}

void AGrandfatherClock::BeginPlay()
{
	Super::BeginPlay();

	// If no custom mesh is assigned, use the antique wardrobe mesh as default
	if (ClockMesh && !ClockMesh->GetStaticMesh())
	{
		UStaticMesh* DefaultMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Props/Furniture/SM_Wardrobe.SM_Wardrobe"));
		if (DefaultMesh)
		{
			ClockMesh->SetStaticMesh(DefaultMesh);
		}
	}
}

bool AGrandfatherClock::CanInteract_Implementation(AHorrorCharacter* Interactor)
{
	return Interactor != nullptr;
}

FText AGrandfatherClock::GetInteractionPrompt_Implementation(AHorrorCharacter* Interactor)
{
	return bSecretRevealed ? FText::FromString(TEXT("Examine Stopped Clock")) : FText::FromString(TEXT("Inspect Clock Mechanism"));
}

void AGrandfatherClock::OnFocusGained_Implementation(AHorrorCharacter* Interactor)
{
	if (ClockMesh)
	{
		ClockMesh->SetRenderCustomDepth(true);
	}
}

void AGrandfatherClock::OnFocusLost_Implementation(AHorrorCharacter* Interactor)
{
	if (ClockMesh)
	{
		ClockMesh->SetRenderCustomDepth(false);
	}
}

void AGrandfatherClock::Interact_Implementation(AHorrorCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!bSecretRevealed)
	{
		bSecretRevealed = true;

		// Play secret compartment sound
		if (SecretCompartmentAudio)
		{
			UGameplayStatics::PlaySoundAtLocation(World, SecretCompartmentAudio, GetActorLocation());
		}

		// Add document to Cross's notebook
		if (UHorrorInventoryComponent* Inv = Interactor->FindComponentByClass<UHorrorInventoryComponent>())
		{
			Inv->AddDocument(HiddenDocument);
			Inv->MarkFactSolved(FName("Fact_StoppedClock"));
		}

		OnClockSecretRevealed.Broadcast();
	}
	else
	{
		// Already found secret, examine clock again
		if (UHorrorInventoryComponent* Inv = Interactor->FindComponentByClass<UHorrorInventoryComponent>())
		{
			Inv->AddDocument(HiddenDocument);
		}
	}
}
