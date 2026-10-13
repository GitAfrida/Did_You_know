// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Horror/Interaction/HorrorInspectable.h"
#include "HorrorKeyPickup.generated.h"

/**
 * Dedicated 3D key pickup for Graves Country House.
 * Preconfigured out-of-the-box: has its 3D model attached, key ID configured, and pickup logic ready.
 */
UCLASS()
class DIDYOUKNOW_API AHorrorKeyPickup : public AHorrorInspectable
{
	GENERATED_BODY()

public:
	AHorrorKeyPickup();
};
