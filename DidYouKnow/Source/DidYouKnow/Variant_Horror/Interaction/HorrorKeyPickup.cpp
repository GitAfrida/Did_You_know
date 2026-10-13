// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Interaction/HorrorKeyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AHorrorKeyPickup::AHorrorKeyPickup()
{
	bIsPickup = true;
	PickupItemId = FName("Key_Study");
	CustomPrompt = FText::FromString(TEXT("Take Study Key"));

	// Pre-load default 3D mesh so it is physically visible in the editor without manual setup
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeyMeshAsset(TEXT("/Game/MafiaBarnPack/Meshes/Props/Tools/SM_Wrench.SM_Wrench"));
	if (KeyMeshAsset.Succeeded() && MeshComponent)
	{
		MeshComponent->SetStaticMesh(KeyMeshAsset.Object);
	}
}
