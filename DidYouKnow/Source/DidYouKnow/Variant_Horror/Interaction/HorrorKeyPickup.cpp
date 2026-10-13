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

	// Pre-load default 3D mesh: real antique key from LivingRoom pack, with fallback to barn pack
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeyMeshAsset(TEXT("/Game/LivingRoom/Meshes/SM_Keys_01a.SM_Keys_01a"));
	if (KeyMeshAsset.Succeeded() && MeshComponent)
	{
		MeshComponent->SetStaticMesh(KeyMeshAsset.Object);
	}
	else
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> FallbackWrenchAsset(TEXT("/Game/MafiaBarnPack/Meshes/Props/Tools/SM_Wrench.SM_Wrench"));
		if (FallbackWrenchAsset.Succeeded() && MeshComponent)
		{
			MeshComponent->SetStaticMesh(FallbackWrenchAsset.Object);
		}
	}
}
