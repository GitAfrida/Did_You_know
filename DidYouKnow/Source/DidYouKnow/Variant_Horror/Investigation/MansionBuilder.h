// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MansionBuilder.generated.h"

class UInstancedStaticMeshComponent;
class UPointLightComponent;
class AHorrorDoor;
class AHorrorKeyPickup;
class AHorrorPortraitClue;
class AGrandfatherClock;
class AThreeDoorPuzzleRoom;

/**
 * Procedural architecture builder for Graves Country House.
 * Automatically constructs the 3-room layout (Grand Foyer, Corridor of Dread, Locked Study, 3-Door Chamber)
 * with exact 300cm modular grid alignment, horror lighting, and wired puzzle mechanics.
 */
UCLASS()
class DIDYOUKNOW_API AMansionBuilder : public AActor
{
	GENERATED_BODY()

public:
	AMansionBuilder();

	/** Clears and rebuilds the complete Graves Country House layout */
	UFUNCTION(CallInEditor, Category = "Mansion Builder")
	void BuildMansion();

	/** Removes all generated walls, floors, ceilings, and spawned gameplay actors */
	UFUNCTION(CallInEditor, Category = "Mansion Builder")
	void ClearMansion();

protected:
	virtual void BeginPlay() override;

	void BuildFloorsAndCeilings();
	void BuildWallsAndArchways();
	void BuildLighting();
	void BuildGameplayActors();

protected:
	// Instanced mesh components for maximum performance (1 draw call per group)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> FloorInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> WallInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> CeilingInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> ArchInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> ColumnInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> WindowInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> BookcaseInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> StairsInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInstancedStaticMeshComponent> PropInstances;

	// Mesh assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> CeilingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> ArchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> ColumnMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> WindowWallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> FireplaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> DeskMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> BookcaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> SofaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> CarpetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> StairsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> ChandelierMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mansion|Meshes")
	TObjectPtr<UStaticMesh> WallLampMesh;

	// Spawned Gameplay Actors
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mansion|Spawned")
	TObjectPtr<AHorrorPortraitClue> SpawnedPortrait;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mansion|Spawned")
	TObjectPtr<AHorrorKeyPickup> SpawnedKey;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mansion|Spawned")
	TObjectPtr<AHorrorDoor> SpawnedStudyDoor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mansion|Spawned")
	TObjectPtr<AGrandfatherClock> SpawnedClock;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mansion|Spawned")
	TObjectPtr<AThreeDoorPuzzleRoom> SpawnedPuzzleRoom;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mansion|Spawned")
	TArray<TObjectPtr<AActor>> SpawnedDoorsAndProps;

	UPROPERTY(SaveGame)
	bool bIsBuilt = false;
};
