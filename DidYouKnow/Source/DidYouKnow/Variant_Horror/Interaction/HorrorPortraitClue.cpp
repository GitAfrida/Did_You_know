// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Interaction/HorrorPortraitClue.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AHorrorPortraitClue::AHorrorPortraitClue()
{
	bIsPickup = false;
	CustomPrompt = FText::FromString(TEXT("Examine Mila's Portrait"));
	TriggerMilaFactId = FName("Fact_MilaPortrait");
	TriggerMilaFactQuote = FText::FromString(TEXT("Did you know that memories linger in cold places? Victor hid the key where time stands still."));
	TriggerMilaFactDeduction = FText::FromString(TEXT("The study key is hidden near the stopped grandfather clock."));

	// Pre-load portrait 3D mesh so it is physically visible in the editor without manual setup
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PortraitMeshAsset(TEXT("/Game/MafiaBarnPack/Meshes/Props/Paper/SM_PhotoPlane1.SM_PhotoPlane1"));
	if (PortraitMeshAsset.Succeeded() && MeshComponent)
	{
		MeshComponent->SetStaticMesh(PortraitMeshAsset.Object);
	}
}
