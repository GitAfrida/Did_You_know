// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Horror/HorrorCharacter.h"
#include "Variant_Horror/Interaction/HorrorInteractionComponent.h"
#include "Variant_Horror/Investigation/HorrorInventoryComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"

AHorrorCharacter::AHorrorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. Create Flashlight Mesh attached to first-person left hand
	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
	FlashlightMesh->SetupAttachment(GetFirstPersonMesh(), FName("FlashLight"));
	FlashlightMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	FlashlightMesh->SetRelativeScale3D(FVector(1.0f));
	FlashlightMesh->SetCollisionProfileName(FName("NoCollision"));
	FlashlightMesh->SetCastShadow(false);
	FlashlightMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FlashlightMesh->SetOnlyOwnerSee(true);
	FlashlightMesh->SetVisibility(false); // Default: unequipped / hidden

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FlashlightMeshAsset(
		TEXT("/Game/FlashLight_System/Mesh/SK_Flashlight.SK_Flashlight")
	);
	if (FlashlightMeshAsset.Succeeded())
	{
		FlashlightMesh->SetStaticMesh(FlashlightMeshAsset.Object);
	}

	// 2. Create SpotLight attached to the physical flashlight mesh lens
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(FlashlightMesh);
	SpotLight->SetRelativeLocationAndRotation(SpotLightRelativeLocation, FRotator(90.0f, 0.0f, 0.0f));
	SpotLight->Intensity = BaseLightIntensity;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 3500.0f;
	SpotLight->InnerConeAngle = 18.0f;
	SpotLight->OuterConeAngle = 36.0f;
	SpotLight->SetVisibility(false); // Starts OFF

	// 3. Audio Assets
	static ConstructorHelpers::FObjectFinder<USoundBase> OnSoundObj(
		TEXT("/Game/FlashLight_System/Sound/Flashlight_On.Flashlight_On")
	);
	if (OnSoundObj.Succeeded())
	{
		FlashlightOnSound = OnSoundObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> OffSoundObj(
		TEXT("/Game/FlashLight_System/Sound/Flashlight_OFF.Flashlight_OFF")
	);
	if (OffSoundObj.Succeeded())
	{
		FlashlightOffSound = OffSoundObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationObj(
		TEXT("/Game/FlashLight_System/Sound/Att_FlashLight.Att_FlashLight")
	);
	if (AttenuationObj.Succeeded())
	{
		FlashlightAttenuation = AttenuationObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> FlickerSoundObj(
		TEXT("/Game/FlashLight_System/Sound/LightFlicker02.LightFlicker02")
	);
	if (FlickerSoundObj.Succeeded())
	{
		LightFlickerSound = FlickerSoundObj.Object;
	}

	// 4. Create interaction and investigation components
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

	// Ensure third-person body mesh ticks even when not rendered so CopyPoseFromMesh receives full locomotion updates
	if (GetMesh())
	{
		GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	}

	// Ensure FP arms mesh ticks after parent body mesh and updates pose reliably
	if (GetFirstPersonMesh())
	{
		GetFirstPersonMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		GetFirstPersonMesh()->AddTickPrerequisiteComponent(GetMesh());
	}

	// Ensure flashlight mesh is correctly snapped to left hand socket and starts HIDDEN
	if (FlashlightMesh && GetFirstPersonMesh())
	{
		if (GetFirstPersonMesh()->DoesSocketExist(TEXT("FlashLight")))
		{
			FlashlightMesh->AttachToComponent(GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FlashLight"));
			FlashlightMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			FlashlightMesh->SetRelativeScale3D(FVector(1.0f));
		}
		else
		{
			FlashlightMesh->AttachToComponent(GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_l"));
			FlashlightMesh->SetRelativeLocationAndRotation(FlashlightGripLocation, FlashlightGripRotation);
			FlashlightMesh->SetRelativeScale3D(FlashlightMeshScale);
		}
		FlashlightMesh->SetOnlyOwnerSee(true);
		FlashlightMesh->SetVisibility(false);
	}

	// Ensure spotlight is attached to flashlight mesh lens and starts OFF
	if (SpotLight && FlashlightMesh)
	{
		SpotLight->AttachToComponent(FlashlightMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SpotLight->SetRelativeLocationAndRotation(SpotLightRelativeLocation, FRotator(90.0f, 0.0f, 0.0f));
		SpotLight->SetIntensity(BaseLightIntensity);
		SpotLight->SetVisibility(false);
	}

	bFlashlightOn = false;
	CurrentFlashlightAlpha = 0.0f;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1001, 12.0f, FColor::Cyan, TEXT("[DID YOU KNOW?] Cross Grover entering Graves Country House"));
		GEngine->AddOnScreenDebugMessage(1002, 12.0f, FColor::Yellow, TEXT("Controls: [WASD] Move | [F] Flashlight ON/OFF | [E] Interact / Examine | [Tab] Notebook"));
	}
}

float AHorrorCharacter::GetFlashlightAimPitch() const
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		return FMath::Clamp(FRotator::NormalizeAxis(PC->GetControlRotation().Pitch), -85.0f, 85.0f);
	}
	return 0.0f;
}

void AHorrorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Smoothly interpolate CurrentFlashlightAlpha [0..1] for raising/lowering the arm
	const float TargetAlpha = bFlashlightOn ? 1.0f : 0.0f;
	CurrentFlashlightAlpha = FMath::FInterpTo(CurrentFlashlightAlpha, TargetAlpha, DeltaTime, FlashlightRaiseInterpSpeed);

	// 2. Control mesh visibility: visible when equipped, hidden when unequipped
	const bool bShouldShowMesh = bFlashlightOn || (CurrentFlashlightAlpha > 0.02f);
	if (FlashlightMesh && FlashlightMesh->IsVisible() != bShouldShowMesh)
	{
		FlashlightMesh->SetVisibility(bShouldShowMesh);
	}

	// 3. Keep first-person AnimInstance synchronized with active state and camera pitch
	if (USkeletalMeshComponent* FPMesh = GetFirstPersonMesh())
	{
		if (UAnimInstance* AnimInst = FPMesh->GetAnimInstance())
		{
			UClass* AnimClass = AnimInst->GetClass();

			auto SetNumericProp = [AnimInst, AnimClass](const FName& PropName, double Val)
			{
				if (FProperty* Prop = AnimClass->FindPropertyByName(PropName))
				{
					if (FNumericProperty* NumProp = CastField<FNumericProperty>(Prop))
					{
						NumProp->SetFloatingPointPropertyValue(Prop->ContainerPtrToValuePtr<void>(AnimInst), Val);
					}
				}
			};

			SetNumericProp(TEXT("FlashlightAlpha"), CurrentFlashlightAlpha);

			if (FBoolProperty* ActiveProp = CastField<FBoolProperty>(AnimClass->FindPropertyByName(TEXT("FlashlightPoseActive"))))
			{
				ActiveProp->SetPropertyValue_InContainer(AnimInst, bFlashlightOn);
			}

			const float Pitch = GetFlashlightAimPitch();
			SetNumericProp(TEXT("FlashlightAimY"), Pitch);
			SetNumericProp(TEXT("FlashlightAimX"), 0.0);
		}
	}

	// 4. Direct hardware key polling: guarantees F (flashlight), E (interact), and Tab/N (notebook)
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

	// clear sprint and flicker timers
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
	GetWorld()->GetTimerManager().ClearTimer(FlickerTimer);
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

	SetFlashlightEquipped(!bFlashlightOn, true);
}

void AHorrorCharacter::SetFlashlightEquipped(bool bEquip, bool bPlaySound)
{
	bFlashlightOn = bEquip;

	if (SpotLight)
	{
		SpotLight->SetVisibility(bFlashlightOn);
		SpotLight->SetIntensity(bFlashlightOn ? BaseLightIntensity : 0.0f);
	}

	if (FlashlightMesh && bFlashlightOn)
	{
		FlashlightMesh->SetVisibility(true);
	}

	if (bPlaySound)
	{
		USoundBase* SoundToPlay = bFlashlightOn ? FlashlightOnSound.Get() : FlashlightOffSound.Get();
		if (SoundToPlay)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				SoundToPlay,
				GetActorLocation(),
				1.0f,
				1.0f,
				0.0f,
				FlashlightAttenuation
			);
		}
	}

	OnFlashlightToggled.Broadcast(bFlashlightOn);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2001, 2.5f, bFlashlightOn ? FColor::Yellow : FColor::Silver,
			FString::Printf(TEXT("[FLASHLIGHT: %s]"), bFlashlightOn ? TEXT("EQUIPPED (ON)") : TEXT("UNEQUIPPED (OFF)")));
	}
}

void AHorrorCharacter::TriggerFlashlightFlicker(float Duration, int32 FlickerCount, bool bTurnOffAfter)
{
	if (!GetWorld() || FlickerCount <= 0)
	{
		return;
	}

	RemainingFlickerSteps = FlickerCount * 2;
	bFlickerTurnOffAtEnd = bTurnOffAfter;

	if (LightFlickerSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightFlickerSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, FlashlightAttenuation);
	}

	const float StepInterval = FMath::Max(Duration / static_cast<float>(RemainingFlickerSteps), 0.04f);
	GetWorld()->GetTimerManager().SetTimer(
		FlickerTimer,
		[this]()
		{
			if (RemainingFlickerSteps <= 0)
			{
				GetWorld()->GetTimerManager().ClearTimer(FlickerTimer);
				SetFlashlightEquipped(!bFlickerTurnOffAtEnd, false);
				return;
			}

			if (SpotLight)
			{
				const bool bStepVisible = (RemainingFlickerSteps % 2 == 0);
				SpotLight->SetVisibility(bStepVisible);
			}
			--RemainingFlickerSteps;
		},
		StepInterval,
		true
	);
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
