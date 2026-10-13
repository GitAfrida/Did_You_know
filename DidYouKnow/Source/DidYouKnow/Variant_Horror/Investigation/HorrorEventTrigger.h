// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorEventTrigger.generated.h"

class UBoxComponent;
class AHorrorDoor;
class USoundBase;
class ALight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorEventTriggered);

/**
 * Trigger volume for scripting psychological horror events:
 * flickering electricity, ceiling footsteps, fleeting apparitions, dialogue, and door slams.
 */
UCLASS()
class DIDYOUKNOW_API AHorrorEventTrigger : public AActor
{
	GENERATED_BODY()

public:
	AHorrorEventTrigger();

	UPROPERTY(BlueprintAssignable, Category = "Horror|Event")
	FOnHorrorEventTriggered OnHorrorEventTriggered;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ResetLightState();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	/** If true, this event only occurs once */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event")
	bool bTriggerOnce = true;

	/** Door to forcefully slam shut when this event triggers */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Event|Door")
	TObjectPtr<AHorrorDoor> DoorToSlam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Door")
	bool bLockDoorAfterSlam = true;

	/** Lights to toggle/flicker off or on */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Event|Lights")
	TArray<TObjectPtr<ALight>> AffectedLights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Lights")
	bool bTurnLightsOff = false;

	/** Audio cues (e.g. electrical click, footsteps from ceiling, scream) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Audio")
	TObjectPtr<USoundBase> EventAudio;

	/** Mila fact triggered by this event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Mila")
	FName MilaFactId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Mila")
	FText MilaFactQuote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Mila")
	FText MilaFactDeduction;

	/** Cross's monologue text (e.g. "Someone's still getting electricity in here?", "Mara...") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Dialogue")
	FText CrossMonologue;

	/** Narrative objective assigned upon this trigger */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Objective")
	FText NewObjective;

	/** Transient ghost actor spawned or shown for a fraction of a second */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Horror|Event|Apparition")
	TObjectPtr<AActor> FleetingApparitionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Event|Apparition")
	float ApparitionDuration = 0.4f;

private:
	bool bHasTriggered = false;
	FTimerHandle ApparitionTimer;
	FTimerHandle LightFlickerTimer;
};
