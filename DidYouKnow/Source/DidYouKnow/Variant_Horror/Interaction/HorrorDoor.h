// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Variant_Horror/Interaction/HorrorInteractableInterface.h"
#include "Variant_Horror/Investigation/HorrorTypes.h"
#include "HorrorDoor.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDoorStateChanged, bool, bIsOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorSlammed);

/**
 * Interactive door with unlocking, creaking open/close physics, and supernatural slamming mechanisms.
 */
UCLASS()
class DIDYOUKNOW_API AHorrorDoor : public AActor, public IHorrorInteractable
{
	GENERATED_BODY()

public:
	AHorrorDoor();

	virtual void Tick(float DeltaTime) override;

	// IHorrorInteractable Interface
	virtual bool CanInteract_Implementation(AHorrorCharacter* Interactor) override;
	virtual void Interact_Implementation(AHorrorCharacter* Interactor) override;
	virtual FText GetInteractionPrompt_Implementation(AHorrorCharacter* Interactor) override;
	virtual void OnFocusGained_Implementation(AHorrorCharacter* Interactor) override;
	virtual void OnFocusLost_Implementation(AHorrorCharacter* Interactor) override;

	/** Smoothly opens the door */
	UFUNCTION(BlueprintCallable, Category = "Horror|Door")
	void OpenDoor();

	/** Smoothly closes the door */
	UFUNCTION(BlueprintCallable, Category = "Horror|Door")
	void CloseDoor();

	/** Toggles door between open and closed */
	UFUNCTION(BlueprintCallable, Category = "Horror|Door")
	void ToggleDoor();

	/**
	 * Instantly or forcefully slams the door shut with a loud BANG sound,
	 * optionally locking it. Crucial for horror and lock-in events.
	 */
	UFUNCTION(BlueprintCallable, Category = "Horror|Door")
	void SlamShut(bool bLockAfterSlam = true);

	/** Unlocks the door */
	UFUNCTION(BlueprintCallable, Category = "Horror|Door")
	void UnlockDoor();

	/** Locks the door */
	UFUNCTION(BlueprintCallable, Category = "Horror|Door")
	void LockDoor(FName WithKey = NAME_None);

	/** Returns whether the door is open */
	UFUNCTION(BlueprintPure, Category = "Horror|Door")
	bool IsOpen() const { return bIsOpen; }

	/** Returns whether the door is currently locked */
	UFUNCTION(BlueprintPure, Category = "Horror|Door")
	bool IsLocked() const { return bIsLocked; }

	UPROPERTY(BlueprintAssignable, Category = "Horror|Door")
	FOnDoorStateChanged OnDoorStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Horror|Door")
	FOnDoorSlammed OnDoorSlammed;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> DoorHinge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	/** Door state flags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door")
	bool bIsOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door")
	bool bIsLocked = false;

	/** Key item ID required to unlock this door (e.g. "Key_Second", "Key_Upstairs") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door", meta = (EditCondition = "bIsLocked"))
	FName RequiredKeyId = NAME_None;

	/** Target angle in degrees when fully opened */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Motion")
	float OpenAngle = 90.0f;

	/** Speed of opening/closing interpolation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Motion")
	float MovementSpeed = 3.0f;

	/** Audio cues */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Audio")
	TObjectPtr<USoundBase> CreakOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Audio")
	TObjectPtr<USoundBase> CreakCloseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Audio")
	TObjectPtr<USoundBase> LockedRattleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Audio")
	TObjectPtr<USoundBase> UnlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horror|Door|Audio")
	TObjectPtr<USoundBase> SlamBangSound;

private:
	float CurrentYaw = 0.0f;
	float TargetYaw = 0.0f;
	bool bIsMoving = false;
};
