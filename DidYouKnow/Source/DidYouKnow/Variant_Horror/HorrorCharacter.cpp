// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Interaction/HorrorInteractionComponent.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

AHorrorCharacter::AHorrorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the spotlight - aligned straight forward with player's first-person gaze
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetFirstPersonCameraComponent());

	SpotLight->SetRelativeLocationAndRotation(FVector(20.0f, 0.0f, -10.0f), FRotator::ZeroRotator);
	SpotLight->Intensity = 12000.0f;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 3500.0f;
	SpotLight->InnerConeAngle = 18.0f;
	SpotLight->OuterConeAngle = 36.0f;
	SpotLight->SetVisibility(false); // Starts OFF so player controls when to turn it on with F

	// create interaction and investigation components
	InteractionComponent = CreateDefaultSubobject<UHorrorInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UHorrorInventoryComponent>(TEXT("InventoryComponent"));
}

void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize sprint meter to max
	SprintMeter = SprintTime;

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHorrorCharacter::SprintFixedTick, SprintFixedTickTime, true);

	// Ensure the flashlight is attached directly to the first-person camera, points straight forward, and starts OFF
	if (SpotLight && GetFirstPersonCameraComponent())
	{
		SpotLight->AttachToComponent(GetFirstPersonCameraComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SpotLight->SetRelativeLocationAndRotation(FVector(20.0f, 0.0f, -10.0f), FRotator::ZeroRotator);
		SpotLight->SetVisibility(false);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1001, 12.0f, FColor::Cyan, TEXT("[DID YOU KNOW?] Cross Grover entering Graves Country House"));
		GEngine->AddOnScreenDebugMessage(1002, 12.0f, FColor::Yellow, TEXT("Controls: [WASD] Move | [F] Flashlight ON/OFF | [E] Interact / Examine | [Tab] Notebook"));
	}
}

void AHorrorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Direct hardware key polling: guarantees F (flashlight), E (interact), and Tab/N (notebook)
	// always work 100% reliably regardless of Enhanced Input asset mapping state
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (PC->WasInputKeyJustPressed(EKeys::F))
		{
			DoToggleFlashlight();
		}

		if (PC->WasInputKeyJustPressed(EKeys::E))
		{
			DoInteract();
		}

		if (PC->WasInputKeyJustPressed(EKeys::Tab) || PC->WasInputKeyJustPressed(EKeys::N))
		{
			DoToggleNotebook();
		}
	}
}

void AHorrorCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the sprint timer
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}

void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		// Fallback direct keybindings so E, F, N, and Tab work out of the box with zero setup
		PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &AHorrorCharacter::DoInteract);
		PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &AHorrorCharacter::DoToggleFlashlight);
		PlayerInputComponent->BindKey(EKeys::N, IE_Pressed, this, &AHorrorCharacter::DoToggleNotebook);
		PlayerInputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &AHorrorCharacter::DoToggleNotebook);

		// Enhanced Input bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Sprinting
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorCharacter::DoEndSprint);

			// Interaction
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoInteract);

			// Flashlight
			EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoToggleFlashlight);

			// Notebook
			EnhancedInputComponent->BindAction(NotebookAction, ETriggerEvent::Started, this, &AHorrorCharacter::DoToggleNotebook);
		}
	}
}

namespace
{
	static double GLastInteractTime = 0.0;
	static double GLastFlashlightToggleTime = 0.0;
	static double GLastNotebookToggleTime = 0.0;
}

void AHorrorCharacter::DoInteract()
{
	const double CurrentTime = FPlatformTime::Seconds();
	if (CurrentTime - GLastInteractTime < 0.08)
	{
		return;
	}
	GLastInteractTime = CurrentTime;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2002, 1.5f, FColor::Emerald, TEXT("[KEY] 'E' Pressed"));
	}

	if (InteractionComponent)
	{
		InteractionComponent->TryInteract();
	}
}

void AHorrorCharacter::DoToggleFlashlight()
{
	const double CurrentTime = FPlatformTime::Seconds();
	if (CurrentTime - GLastFlashlightToggleTime < 0.25)
	{
		return;
	}
	GLastFlashlightToggleTime = CurrentTime;

	if (SpotLight)
	{
		const bool bNewVisibility = !SpotLight->IsVisible();
		SpotLight->SetVisibility(bNewVisibility);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(2001, 3.0f, bNewVisibility ? FColor::Yellow : FColor::Silver, FString::Printf(TEXT("[FLASHLIGHT: %s]"), bNewVisibility ? TEXT("ON") : TEXT("OFF")));
		}
	}
}

void AHorrorCharacter::DoToggleNotebook()
{
	const double CurrentTime = FPlatformTime::Seconds();
	if (CurrentTime - GLastNotebookToggleTime < 0.25)
	{
		return;
	}
	GLastNotebookToggleTime = CurrentTime;

	OnNotebookToggleRequested.Broadcast();
}

void AHorrorCharacter::DoStartSprint()
{
	// set the sprinting flag
	bSprinting = true;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the sprint walk speed
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(true);
	}

}

void AHorrorCharacter::DoEndSprint()
{
	// set the sprinting flag
	bSprinting = false;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the default walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		// call the sprint state changed delegate
		OnSprintStateChanged.Broadcast(false);
	}
}

void AHorrorCharacter::SprintFixedTick()
{
	// are we out of recovery, still have stamina and are moving faster than our walk speed?
	if (bSprinting && !bRecovering && GetVelocity().Length() > WalkSpeed)
	{

		// do we still have meter to burn?
		if (SprintMeter > 0.0f)
		{
			// update the sprint meter
			SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);

			// have we run out of stamina?
			if (SprintMeter <= 0.0f)
			{
				// raise the recovering flag
				bRecovering = true;

				// set the recovering walk speed
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}
		
	} else {

		// recover stamina
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (SprintMeter >= SprintTime)
		{
			// lower the recovering flag
			bRecovering = false;

			// set the walk or sprint speed depending on whether the sprint button is down
			GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;

			// update the sprint state depending on whether the button is down or not
			OnSprintStateChanged.Broadcast(bSprinting);
		}

	}

	// broadcast the sprint meter updated delegate
	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);

}
