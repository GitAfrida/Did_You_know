// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DidYouKnowCharacter.h"
#include "HorrorCharacter.generated.h"

class USpotLightComponent;
class UInputAction;
class UHorrorInteractionComponent;
class UHorrorInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeterDelegate, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChangedDelegate, bool, bSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotebookToggleRequested);

/**
 *  First person horror protagonist (Cross Grover)
 *  Provides stamina sprint, flashlight, interaction, and investigation notebook
 */
UCLASS(abstract)
class DIDYOUKNOW_API AHorrorCharacter : public ADidYouKnowCharacter
{
	GENERATED_BODY()

	/** Player light source */
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

public:

	/** Delegate called when the sprint meter should be updated */
	FUpdateSprintMeterDelegate OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FSprintStateChangedDelegate OnSprintStateChanged;

protected:

	/** Constructor */
	AHorrorCharacter();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Frame tick for input polling and responsive interaction */
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

	/** Toggles Cross's flashlight / spotlight on and off */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoToggleFlashlight();

	/** Requests opening or closing the detective notebook */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoToggleNotebook();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();

public:
	/** Getters for horror components */
	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	UHorrorInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	UHorrorInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UFUNCTION(BlueprintPure, Category = "Horror|Components")
	USpotLightComponent* GetSpotLight() const { return SpotLight; }

	/** Broadcast when player presses notebook toggle button */
	UPROPERTY(BlueprintAssignable, Category = "Horror|UI")
	FOnNotebookToggleRequested OnNotebookToggleRequested;
};
