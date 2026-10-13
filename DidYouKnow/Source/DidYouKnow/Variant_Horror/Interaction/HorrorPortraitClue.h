// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Variant_Horror/Interaction/HorrorInspectable.h"
#include "HorrorPortraitClue.generated.h"

/**
 * Dedicated 3D portrait clue for Mila Graves in Graves Country House.
 * Preconfigured out-of-the-box: has its portrait 3D model, examination prompt, and Mila fact clue.
 */
UCLASS()
class DIDYOUKNOW_API AHorrorPortraitClue : public AHorrorInspectable
{
	GENERATED_BODY()

public:
	AHorrorPortraitClue();
};
