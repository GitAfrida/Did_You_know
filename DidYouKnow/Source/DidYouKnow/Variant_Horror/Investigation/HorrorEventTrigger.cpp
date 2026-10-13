// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/HorrorEventTrigger.h"
#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Interaction/HorrorDoor.h"
#include "Variant_Horror/Investigation/MilaDialogueSubsystem.h"
#include "Variant_Horror/Investigation/HorrorObjectiveSubsystem.h"
#include "Components/BoxComponent.h"
#include "Engine/Light.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

AHorrorEventTrigger::AHorrorEventTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

void AHorrorEventTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHorrorEventTrigger::OnTriggerOverlap);

	// Ensure fleeting apparition starts hidden
	if (FleetingApparitionActor)
	{
		FleetingApparitionActor->SetActorHiddenInGame(true);
	}
}

void AHorrorEventTrigger::OnTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bTriggerOnce && bHasTriggered)
	{
		return;
	}

	AHorrorCharacter* Character = Cast<AHorrorCharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	bHasTriggered = true;
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 1. Play event sound effect (click, slam, or ceiling footsteps)
	if (EventAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(World, EventAudio, GetActorLocation());
	}

	// 2. Door slam
	if (DoorToSlam)
	{
		DoorToSlam->SlamShut(bLockDoorAfterSlam);
	}

	// 3. Toggle lights
	for (ALight* LightActor : AffectedLights)
	{
		if (LightActor)
		{
			LightActor->SetActorHiddenInGame(bTurnLightsOff);
		}
	}

	// 4. Fleeting apparition
	if (FleetingApparitionActor)
	{
		FleetingApparitionActor->SetActorHiddenInGame(false);
		World->GetTimerManager().SetTimer(ApparitionTimer, [this]()
		{
			if (FleetingApparitionActor)
			{
				FleetingApparitionActor->SetActorHiddenInGame(true);
			}
		}, ApparitionDuration, false);
	}

	// 5. Mila fact trigger
	if (!MilaFactId.IsNone() && !MilaFactQuote.IsEmpty())
	{
		if (UMilaDialogueSubsystem* MilaSub = World->GetSubsystem<UMilaDialogueSubsystem>())
		{
			MilaSub->TriggerMilaFact(MilaFactId, MilaFactQuote, MilaFactDeduction);
		}
	}

	// 6. Cross monologue
	if (!CrossMonologue.IsEmpty())
	{
		if (UMilaDialogueSubsystem* MilaSub = World->GetSubsystem<UMilaDialogueSubsystem>())
		{
			MilaSub->TriggerDirectDialogue(FText::FromString(TEXT("Cross")), CrossMonologue);
		}
	}

	// 7. Objective update
	if (!NewObjective.IsEmpty())
	{
		if (UHorrorObjectiveSubsystem* ObjSub = World->GetSubsystem<UHorrorObjectiveSubsystem>())
		{
			ObjSub->SetObjective(NewObjective);
		}
	}

	OnHorrorEventTriggered.Broadcast();
}
