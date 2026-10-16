// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DidYouKnowCharacter.h"
#include "HorrorCharacter.generated.h"

class USpotLightComponent;
class UStaticMeshComponent;
class USoundBase;
class USoundAttenuation;
class UInputAction;
class UHorrorInteractionComponent;
class UHorrorInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChangedDelegate, bool, bSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotebookToggleRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlashlightToggledDelegate, bool, bEquipped);

/**
 *  First person horror protagonist (Cross Grover)
 *  Provides stamina sprint, flashlight, interaction, and investigation notebook
 */
UCLASS(abstract)
class DIDYOUKNOW_API AHorrorCharacter : public ADidYouKnowCharacter
{
	GENERATED_BODY()

	/** 3D Flashlight physical mesh attached to left hand */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FlashlightMesh;

	/** Player light source attached to the flashlight lens */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpotLightComponent> SpotLight;

	/** First-person raycasting and interaction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHorrorInteractionComponent> InteractionComponent;

	/** Cross Grover's inventory and notebook records */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHorrorInventoryComponent> InventoryComponent;
	
protected:

	/** Sprint input action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> SprintAction;

	/** Interact input action (E key) */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> InteractAction;

	/** Flashlight toggle input action (F key) */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> FlashlightAction;

	/** Detective notebook toggle input action (N / Tab key) */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> NotebookAction;

	/** If true, we're sprinting */
	bool bSprinting = false;

	/** If true, we're recovering stamina */
	bool bRecovering = false;

	/** Default walk speed when not sprinting or recovering */
	UPROPERTY(EditAnywhere, Category="Walk")
	float WalkSpeed = 250.0f;

	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 3.0f;

	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float SprintSpeed = 600.0f;

	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	/** Sprint tick timer */
	FTimerHandle SprintTimer;

	/** Atmospheric flicker effect timer */
	FTimerHandle FlickerTimer;
	int32 RemainingFlickerSteps = 0;
	bool bFlickerTurnOffAtEnd = false;

public:

	/** Audio assets from Content/FlashLight_System/Sound/ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Audio")
	TObjectPtr<USoundBase> FlashlightOnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Audio")
	TObjectPtr<USoundBase> FlashlightOffSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Audio")
	TObjectPtr<USoundAttenuation> FlashlightAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Audio")
	TObjectPtr<USoundBase> LightFlickerSound;

	/** FlashLight socket offset, relative to hand_r */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Grip")
	FVector FlashlightGripLocation = FVector(7.816962f, -2.510197f, 0.729601f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Grip")
	FRotator FlashlightGripRotation = FRotator(8.410056f, -176.13915f, 23.382326f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Grip")
	FVector FlashlightMeshScale = FVector(1.2230066f);

	/** SpotLight offset relative to FlashlightMesh lens tip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Optics")
	FVector SpotLightRelativeLocation = FVector(0.0f, 0.0f, 16.0f);

	/** Beam intensity in Lumens */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Optics")
	float BaseLightIntensity = 12000.0f;

	/** If true, the flashlight is equipped and turned ON */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Flashlight")
	bool bFlashlightOn = false;

	/** Smoothly-interpolated weight [0..1] driving right-arm raise/lower (read by ABP_FP_Copy) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Horror|Flashlight")
	float CurrentFlashlightAlpha = 0.0f;

	/** Speed at which the arm raises and lowers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Flashlight|Animation")
	float FlashlightRaiseInterpSpeed = 8.0f;

	/** Delegate called when the sprint meter should be updated */
	FUpdateSprintMeterDelegate OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FSprintStateChangedDelegate OnSprintStateChanged;

	/** Delegate called when the flashlight is equipped or unequipped */
	UPROPERTY(BlueprintAssignable, Category = "Horror|Flashlight")
	FOnFlashlightToggledDelegate OnFlashlightToggled;

	/** Broadcast when player presses notebook toggle button */
	UPROPERTY(BlueprintAssignable, Category = "Horror|UI")
	FOnNotebookToggleRequested OnNotebookToggleRequested;

protected:

	/** Constructor */
	AHorrorCharacter();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Frame tick for input polling, animation alpha interpolation, and responsive interaction */
	virtual void Tick(float DeltaTime) override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

protected:

	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoEndSprint();

	/** Interacts with the currently focused world object */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoInteract();

	/** Toggles Cross's flashlight equipped state and raised hand animation */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoToggleFlashlight();

	/** Requests opening or closing the detective notebook */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoToggleNotebook();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();

public:
	/** Equips or unequips the flashlight with sound and arm raise animation */
	UFUNCTION(BlueprintCallable, Category = "Horror|Flashlight")
	void SetFlashlightEquipped(bool bEquip, bool bPlaySound = true);

	/** Returns true if the flashlight is currently equipped and ON */
	UFUNCTION(BlueprintPure, Category = "Horror|Flashlight")
	bool IsFlashlightOn() const { return bFlashlightOn; }

	/** Returns the current smooth arm raise/lower alpha [0..1] */
	UFUNCTION(BlueprintPure, Category = "Horror|Flashlight")
	float GetFlashlightAlpha() const { return CurrentFlashlightAlpha; }

	/** Returns camera aim pitch clamped to [-85, 85] */
	UFUNCTION(BlueprintPure, Category = "Horror|Flashlight")
	float GetFlashlightAimPitch() const;

	/** Triggers an atmospheric horror flicker effect */
	UFUNCTION(BlueprintCallable, Category = "Horror|Flashlight")
	void TriggerFlashlightFlicker(float Duration = 1.0f, int32 FlickerCount = 4, bool bTurnOffAfter = false);

	/** Getters for horror components */
	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	UHorrorInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	UHorrorInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	USpotLightComponent* GetSpotLight() const { return SpotLight; }

	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	UStaticMeshComponent* GetFlashlightMesh() const { return FlashlightMesh; }
};
