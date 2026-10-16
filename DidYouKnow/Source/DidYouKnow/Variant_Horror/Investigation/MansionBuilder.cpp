// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Horror/Investigation/MansionBuilder.h"
#include "Variant_Horror/Interaction/HorrorDoor.h"
#include "Variant_Horror/Interaction/HorrorKeyPickup.h"
#include "Variant_Horror/Interaction/HorrorPortraitClue.h"
#include "Variant_Horror/Investigation/GrandfatherClock.h"
#include "Variant_Horror/Investigation/ThreeDoorPuzzleRoom.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/PointLight.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AMansionBuilder::AMansionBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("MansionRoot"));
	RootComponent = RootComp;

	FloorInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FloorInstances"));
	FloorInstances->SetupAttachment(RootComponent);
	FloorInstances->SetCollisionProfileName(TEXT("BlockAll"));

	WallInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstances"));
	WallInstances->SetupAttachment(RootComponent);
	WallInstances->SetCollisionProfileName(TEXT("BlockAll"));

	CeilingInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CeilingInstances"));
	CeilingInstances->SetupAttachment(RootComponent);
	CeilingInstances->SetCollisionProfileName(TEXT("BlockAll"));

	ArchInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ArchInstances"));
	ArchInstances->SetupAttachment(RootComponent);
	ArchInstances->SetCollisionProfileName(TEXT("BlockAll"));

	ColumnInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ColumnInstances"));
	ColumnInstances->SetupAttachment(RootComponent);
	ColumnInstances->SetCollisionProfileName(TEXT("BlockAll"));

	WindowInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WindowInstances"));
	WindowInstances->SetupAttachment(RootComponent);
	WindowInstances->SetCollisionProfileName(TEXT("BlockAll"));

	BookcaseInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BookcaseInstances"));
	BookcaseInstances->SetupAttachment(RootComponent);
	BookcaseInstances->SetCollisionProfileName(TEXT("BlockAll"));

	StairsInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StairsInstances"));
	StairsInstances->SetupAttachment(RootComponent);
	StairsInstances->SetCollisionProfileName(TEXT("BlockAll"));

	PropInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PropInstances"));
	PropInstances->SetupAttachment(RootComponent);
	PropInstances->SetCollisionProfileName(TEXT("BlockAll"));

	// Pre-load default modular 3D meshes from LivingRoom pack
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FloorAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Floor_3M_01a.SM_BLD_Floor_3M_01a"));
	if (FloorAsset.Succeeded()) FloorMesh = FloorAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_3M_01a.SM_BLD_Wall_3M_01a"));
	if (WallAsset.Succeeded()) WallMesh = WallAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CeilingAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Ceiling_3M_02a.SM_BLD_Ceiling_3M_02a"));
	if (CeilingAsset.Succeeded()) CeilingMesh = CeilingAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArchAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Arch_2M.SM_BLD_Arch_2M"));
	if (ArchAsset.Succeeded()) ArchMesh = ArchAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ColumnAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Column_01b.SM_BLD_Column_01b"));
	if (ColumnAsset.Succeeded()) ColumnMesh = ColumnAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WindowAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_Window_3M.SM_BLD_Wall_Window_3M"));
	if (WindowAsset.Succeeded()) WindowWallMesh = WindowAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FireplaceAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_Fireplace_2M.SM_BLD_Wall_Fireplace_2M"));
	if (FireplaceAsset.Succeeded()) FireplaceMesh = FireplaceAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DeskAsset(TEXT("/Game/LivingRoom/Meshes/SM_Table_01a.SM_Table_01a"));
	if (DeskAsset.Succeeded()) DeskMesh = DeskAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BookcaseAsset(TEXT("/Game/LivingRoom/Meshes/SM_Wall_Bookcase_01a.SM_Wall_Bookcase_01a"));
	if (BookcaseAsset.Succeeded()) BookcaseMesh = BookcaseAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SofaAsset(TEXT("/Game/LivingRoom/Meshes/SM_Sofa_2M_01a.SM_Sofa_2M_01a"));
	if (SofaAsset.Succeeded()) SofaMesh = SofaAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CarpetAsset(TEXT("/Game/LivingRoom/Meshes/SM_Carpet_01a.SM_Carpet_01a"));
	if (CarpetAsset.Succeeded()) CarpetMesh = CarpetAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StairsAsset(TEXT("/Game/LivingRoom/Meshes/SM_BLD_Stairs_01a.SM_BLD_Stairs_01a"));
	if (StairsAsset.Succeeded()) StairsMesh = StairsAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChandelierAsset(TEXT("/Game/LivingRoom/Meshes/SM_Lamp_Top_01d.SM_Lamp_Top_01d"));
	if (ChandelierAsset.Succeeded()) ChandelierMesh = ChandelierAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallLampAsset(TEXT("/Game/LivingRoom/Meshes/SM_Lamp_Wall_01e.SM_Lamp_Wall_01e"));
	if (WallLampAsset.Succeeded()) WallLampMesh = WallLampAsset.Object;
}

void AMansionBuilder::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsBuilt)
	{
		BuildMansion();
	}
}

void AMansionBuilder::ClearMansion()
{
	// Clean up all instanced mesh components
	if (FloorInstances) FloorInstances->ClearInstances();
	if (WallInstances) WallInstances->ClearInstances();
	if (CeilingInstances) CeilingInstances->ClearInstances();
	if (ArchInstances) ArchInstances->ClearInstances();
	if (ColumnInstances) ColumnInstances->ClearInstances();
	if (WindowInstances) WindowInstances->ClearInstances();
	if (BookcaseInstances) BookcaseInstances->ClearInstances();
	if (StairsInstances) StairsInstances->ClearInstances();
	if (PropInstances) PropInstances->ClearInstances();

	// Destroy all individually spawned editable actors
	for (AActor* SpawnedActor : SpawnedDoorsAndProps)
	{
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->Destroy();
		}
	}
	SpawnedDoorsAndProps.Empty();

	if (IsValid(SpawnedPortrait)) { SpawnedPortrait->Destroy(); SpawnedPortrait = nullptr; }
	if (IsValid(SpawnedKey)) { SpawnedKey->Destroy(); SpawnedKey = nullptr; }
	if (IsValid(SpawnedStudyDoor)) { SpawnedStudyDoor->Destroy(); SpawnedStudyDoor = nullptr; }
	if (IsValid(SpawnedClock)) { SpawnedClock->Destroy(); SpawnedClock = nullptr; }
	if (IsValid(SpawnedPuzzleRoom)) { SpawnedPuzzleRoom->Destroy(); SpawnedPuzzleRoom = nullptr; }

	// Remove spawned point lights
	TArray<UPointLightComponent*> Lights;
	GetComponents<UPointLightComponent>(Lights);
	for (UPointLightComponent* Light : Lights)
	{
		Light->DestroyComponent();
	}

	bIsBuilt = false;
}

void AMansionBuilder::BuildMansion()
{
	ClearMansion();

	BuildFloorsAndCeilings();
	BuildWallsAndArchways();
	BuildLighting();
	BuildGameplayActors();

	bIsBuilt = true;
}

void AMansionBuilder::BuildFloorsAndCeilings()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Helper to spawn an editable AStaticMeshActor
	auto SpawnStaticMesh = [&](UStaticMesh* Mesh, const FVector& RelLoc, const FRotator& RelRot, const FName& Folder, const FString& Label, const FVector& Scale = FVector::OneVector) -> AStaticMeshActor*
	{
		if (!Mesh) return nullptr;
		AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(Origin + RelLoc, RelRot, SpawnParams);
		if (Actor)
		{
			UStaticMeshComponent* SMC = Actor->GetStaticMeshComponent();
			if (SMC)
			{
				SMC->SetStaticMesh(Mesh);
				SMC->SetMobility(EComponentMobility::Movable);
				SMC->SetWorldScale3D(Scale);
				SMC->SetCollisionProfileName(TEXT("BlockAll"));
			}
			Actor->SetActorLabel(Label);
			Actor->SetFolderPath(Folder);
			SpawnedDoorsAndProps.Add(Actor);
		}
		return Actor;
	};

	// Load modular assets
	UStaticMesh* LoadedFloor = FloorMesh ? FloorMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Floor_3M_01a.SM_BLD_Floor_3M_01a"));
	UStaticMesh* LoadedCeiling = CeilingMesh ? CeilingMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Ceiling_3M_02a.SM_BLD_Ceiling_3M_02a"));
	UStaticMesh* FoundationMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Architecture/SM_FoundWall.SM_FoundWall"));
	UStaticMesh* RoofSectionMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Architecture/SM_Barn_Roof_Section.SM_Barn_Roof_Section"));
	UStaticMesh* RoofRidgeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Architecture/SM_Barn_RoofRidge.SM_Barn_RoofRidge"));

	// =========================================================================
	// 1. EXTERIOR FOUNDATION & COURTYARD GROUNDS (Editable in Mansion/Exterior)
	// =========================================================================

	// Large estate ground courtyard in front of the house: X in [-300..1200], Y in [-1200..-300] at Z = -10
	for (int32 gx = -1; gx <= 3; ++gx)
	{
		for (int32 gy = -4; gy <= -1; ++gy)
		{
			SpawnStaticMesh(LoadedFloor, FVector(gx * 300.0f, gy * 300.0f, -10.0f), FRotator::ZeroRotator, FName("Mansion/Exterior/Ground"), FString::Printf(TEXT("Courtyard_Paving_%d_%d"), gx, gy));
		}
	}

	// Stone foundation plinths underneath the main building footprint:
	for (int32 gx = -1; gx <= 4; ++gx)
	{
		SpawnStaticMesh(FoundationMesh, FVector(gx * 300.0f, -15.0f, -20.0f), FRotator(0.0f, 0.0f, 0.0f), FName("Mansion/Exterior/Foundation"), FString::Printf(TEXT("Foundation_South_%d"), gx), FVector(1.0f, 1.2f, 0.8f));
		SpawnStaticMesh(FoundationMesh, FVector(gx * 300.0f, 2415.0f, -20.0f), FRotator(0.0f, 180.0f, 0.0f), FName("Mansion/Exterior/Foundation"), FString::Printf(TEXT("Foundation_North_%d"), gx), FVector(1.0f, 1.2f, 0.8f));
	}

	// =========================================================================
	// 2. INTERIOR FLOORS & CEILINGS (Editable in Mansion/Floors & Mansion/Ceilings)
	// =========================================================================

	auto AddRoomCell = [&](int32 gx, int32 gy, const FString& RoomName)
	{
		// Floor tile: covers [gx*300..(gx+1)*300] x [gy*300..(gy+1)*300]
		SpawnStaticMesh(LoadedFloor, FVector(gx * 300.0f, gy * 300.0f, 0.0f), FRotator::ZeroRotator, FName("Mansion/Floors"), FString::Printf(TEXT("Floor_%s_%d_%d"), *RoomName, gx, gy));

		// Ceiling tile: SM_BLD_Ceiling_3M_02a extends -300 in X, +300 in Y, sits at height Z ≈ 298-305
		SpawnStaticMesh(LoadedCeiling, FVector((gx + 1) * 300.0f, gy * 300.0f, 0.0f), FRotator::ZeroRotator, FName("Mansion/Ceilings"), FString::Printf(TEXT("Ceiling_%s_%d_%d"), *RoomName, gx, gy));
	};

	// Room 1: Grand Foyer (3x3 grid = 9m x 9m)
	for (int32 gx = 0; gx <= 2; ++gx)
	{
		for (int32 gy = 0; gy <= 2; ++gy)
		{
			AddRoomCell(gx, gy, TEXT("Foyer"));
		}
	}

	// The Corridor of Dread (1x3 grid = 3m x 9m)
	for (int32 gy = 3; gy <= 5; ++gy)
	{
		AddRoomCell(1, gy, TEXT("Corridor"));
	}

	// Room 2: Victor's Study (2x2 grid = 6m x 6m)
	for (int32 gx = 2; gx <= 3; ++gx)
	{
		for (int32 gy = 3; gy <= 4; ++gy)
		{
			AddRoomCell(gx, gy, TEXT("Study"));
		}
	}

	// Room 3: 3-Door Riddle Chamber (3x2 grid = 9m x 6m)
	for (int32 gx = 0; gx <= 2; ++gx)
	{
		for (int32 gy = 6; gy <= 7; ++gy)
		{
			AddRoomCell(gx, gy, TEXT("RiddleChamber"));
		}
	}

	// Backing alcoves behind the 3 Riddle Doors (1x1 each at gy = 8) so open doors reveal enclosed rooms:
	for (int32 gx = 0; gx <= 2; ++gx)
	{
		AddRoomCell(gx, 8, TEXT("Vestibule"));
	}

	// =========================================================================
	// 3. PITCHED VICTORIAN ROOF ARCHITECTURE (Editable in Mansion/Roof)
	// =========================================================================
	if (RoofSectionMesh)
	{
		// Pitched roof sloping up over the Grand Foyer and Riddle Chamber (Z = 300 to 450)
		for (float Y = 0.0f; Y <= 2400.0f; Y += 400.0f)
		{
			// West roof pitch (sloping up from West to Center):
			SpawnStaticMesh(RoofSectionMesh, FVector(-50.0f, Y, 300.0f), FRotator(25.0f, 0.0f, 0.0f), FName("Mansion/Roof"), FString::Printf(TEXT("Roof_Pitch_West_%.0f"), Y), FVector(1.1f, 1.0f, 1.0f));

			// East roof pitch (sloping up from East to Center):
			SpawnStaticMesh(RoofSectionMesh, FVector(950.0f, Y, 300.0f), FRotator(-25.0f, 180.0f, 0.0f), FName("Mansion/Roof"), FString::Printf(TEXT("Roof_Pitch_East_%.0f"), Y), FVector(1.1f, 1.0f, 1.0f));
		}
	}

	if (RoofRidgeMesh)
	{
		// Central roof ridge running along the spine of the house at Z = 460
		for (float Y = 0.0f; Y <= 2400.0f; Y += 300.0f)
		{
			SpawnStaticMesh(RoofRidgeMesh, FVector(450.0f, Y, 460.0f), FRotator::ZeroRotator, FName("Mansion/Roof"), FString::Printf(TEXT("Roof_Ridge_%.0f"), Y));
		}
	}
}

void AMansionBuilder::BuildWallsAndArchways()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto SpawnStaticMesh = [&](UStaticMesh* Mesh, const FVector& RelLoc, const FRotator& RelRot, const FName& Folder, const FString& Label, const FVector& Scale = FVector::OneVector) -> AStaticMeshActor*
	{
		if (!Mesh) return nullptr;
		AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(Origin + RelLoc, RelRot, SpawnParams);
		if (Actor)
		{
			UStaticMeshComponent* SMC = Actor->GetStaticMeshComponent();
			if (SMC)
			{
				SMC->SetStaticMesh(Mesh);
				SMC->SetMobility(EComponentMobility::Movable);
				SMC->SetWorldScale3D(Scale);
				SMC->SetCollisionProfileName(TEXT("BlockAll"));
			}
			Actor->SetActorLabel(Label);
			Actor->SetFolderPath(Folder);
			SpawnedDoorsAndProps.Add(Actor);
		}
		return Actor;
	};

	// Load modular assets
	UStaticMesh* W_Wall = WallMesh ? WallMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_3M_01a.SM_BLD_Wall_3M_01a"));
	UStaticMesh* W_Window = WindowWallMesh ? WindowWallMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_Window_3M.SM_BLD_Wall_Window_3M"));
	UStaticMesh* W_Arch = ArchMesh ? ArchMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Arch_2M.SM_BLD_Arch_2M"));
	UStaticMesh* W_Column = ColumnMesh ? ColumnMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Column_01b.SM_BLD_Column_01b"));
	UStaticMesh* W_ColumnTall = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Column_01a.SM_BLD_Column_01a"));
	UStaticMesh* W_Fireplace = FireplaceMesh ? FireplaceMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_Fireplace_2M.SM_BLD_Wall_Fireplace_2M"));
	UStaticMesh* W_Stairs = StairsMesh ? StairsMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Stairs_01a.SM_BLD_Stairs_01a"));
	UStaticMesh* W_ExteriorLightBlock = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_BLD_Wall_LightBlock_01a.SM_BLD_Wall_LightBlock_01a"));

	// Wall Placement Helpers (exact cardinal bounds):
	auto AddSouthWall = [&](int32 gx, int32 gy, UStaticMesh* Mesh, const FString& Label)
	{
		SpawnStaticMesh(Mesh, FVector(gx * 300.0f, gy * 300.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FName("Mansion/Walls"), Label);
	};

	auto AddNorthWall = [&](int32 gx, int32 gy, UStaticMesh* Mesh, const FString& Label)
	{
		SpawnStaticMesh(Mesh, FVector((gx + 1) * 300.0f, (gy + 1) * 300.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f), FName("Mansion/Walls"), Label);
	};

	auto AddEastWall = [&](int32 gx, int32 gy, UStaticMesh* Mesh, const FString& Label)
	{
		SpawnStaticMesh(Mesh, FVector((gx + 1) * 300.0f, gy * 300.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Walls"), Label);
	};

	auto AddWestWall = [&](int32 gx, int32 gy, UStaticMesh* Mesh, const FString& Label)
	{
		SpawnStaticMesh(Mesh, FVector(gx * 300.0f, (gy + 1) * 300.0f, 0.0f), FRotator(0.0f, 270.0f, 0.0f), FName("Mansion/Walls"), Label);
	};

	auto AddColumn = [&](int32 gx, int32 gy, const FString& Label)
	{
		SpawnStaticMesh(W_Column, FVector(gx * 300.0f, gy * 300.0f, 0.0f), FRotator::ZeroRotator, FName("Mansion/Columns"), Label);
	};

	// =========================================================================
	// 1. GRAND FRONT ENTRANCE PORTICO & STAIRS
	// =========================================================================
	// Wide stone entrance stairs leading from courtyard (Z = -10) up to front doors:
	SpawnStaticMesh(W_Stairs, FVector(450.0f, -80.0f, -10.0f), FRotator(0.0f, 0.0f, 0.0f), FName("Mansion/Exterior/Entrance"), TEXT("Entrance_Stairs_Main"), FVector(1.4f, 1.2f, 1.0f));

	// Grand entrance portico stone pillars flanking the steps:
	SpawnStaticMesh(W_ColumnTall, FVector(280.0f, -120.0f, -10.0f), FRotator::ZeroRotator, FName("Mansion/Exterior/Entrance"), TEXT("Portico_Pillar_Left"));
	SpawnStaticMesh(W_ColumnTall, FVector(620.0f, -120.0f, -10.0f), FRotator::ZeroRotator, FName("Mansion/Exterior/Entrance"), TEXT("Portico_Pillar_Right"));

	// =========================================================================
	// 2. ROOM 1: GRAND FOYER PERIMETER
	// =========================================================================
	// South Wall (Main entrance):
	AddSouthWall(0, 0, W_Wall, TEXT("Foyer_Wall_South_Left"));
	AddSouthWall(1, 0, W_Arch, TEXT("Foyer_Arch_FrontEntrance")); // Grand Entrance Arch
	AddSouthWall(2, 0, W_Wall, TEXT("Foyer_Wall_South_Right"));

	// West Wall (Exterior window bays):
	AddWestWall(0, 0, W_Window, TEXT("Foyer_Window_West_0"));
	AddWestWall(0, 1, W_Wall, TEXT("Foyer_Wall_West_1"));
	AddWestWall(0, 2, W_Window, TEXT("Foyer_Window_West_2"));

	// East Wall:
	AddEastWall(2, 0, W_Wall, TEXT("Foyer_Wall_East_0"));
	AddEastWall(2, 1, W_Window, TEXT("Foyer_Window_East_1"));
	AddEastWall(2, 2, W_Wall, TEXT("Foyer_Wall_East_2"));

	// North Wall (Fireplace hearth + corridor transition):
	AddNorthWall(0, 2, W_Fireplace, TEXT("Foyer_Fireplace_Wall"));
	AddNorthWall(1, 2, W_Arch, TEXT("Foyer_Arch_ToCorridor"));
	AddNorthWall(2, 2, W_Wall, TEXT("Foyer_Wall_North_Right"));

	// Foyer Ornate Columns:
	AddColumn(0, 0, TEXT("Column_Foyer_SW"));
	AddColumn(3, 0, TEXT("Column_Foyer_SE"));
	AddColumn(0, 3, TEXT("Column_Foyer_NW"));
	AddColumn(3, 3, TEXT("Column_Foyer_NE"));
	AddColumn(1, 0, TEXT("Column_Foyer_Entrance_L"));
	AddColumn(2, 0, TEXT("Column_Foyer_Entrance_R"));
	AddColumn(1, 3, TEXT("Column_Foyer_Corridor_L"));
	AddColumn(2, 3, TEXT("Column_Foyer_Corridor_R"));

	// =========================================================================
	// 3. THE CORRIDOR OF DREAD
	// =========================================================================
	// West Wall (Lined with bookcases):
	AddWestWall(1, 3, W_Wall, TEXT("Corridor_Wall_West_3"));
	AddWestWall(1, 4, W_Wall, TEXT("Corridor_Wall_West_4"));
	AddWestWall(1, 5, W_Wall, TEXT("Corridor_Wall_West_5"));

	// East Wall (Leads into Study at gy = 4):
	AddEastWall(1, 3, W_Wall, TEXT("Corridor_Wall_East_3"));
	AddEastWall(1, 4, W_Arch, TEXT("Corridor_Arch_ToStudy"));
	AddEastWall(1, 5, W_Wall, TEXT("Corridor_Wall_East_5"));

	// North Wall (Transition into Riddle Chamber):
	AddNorthWall(1, 5, W_Arch, TEXT("Corridor_Arch_ToChamber"));

	// Corridor Columns:
	AddColumn(1, 4, TEXT("Column_Corridor_1"));
	AddColumn(2, 4, TEXT("Column_Corridor_2"));
	AddColumn(1, 5, TEXT("Column_Corridor_3"));
	AddColumn(2, 5, TEXT("Column_Corridor_4"));
	AddColumn(1, 6, TEXT("Column_Corridor_5"));
	AddColumn(2, 6, TEXT("Column_Corridor_6"));

	// =========================================================================
	// 4. ROOM 2: VICTOR'S LOCKED STUDY
	// =========================================================================
	// South Wall:
	AddSouthWall(2, 3, W_Wall, TEXT("Study_Wall_South_0"));
	AddSouthWall(3, 3, W_Wall, TEXT("Study_Wall_South_1"));

	// East Wall:
	AddEastWall(3, 3, W_Window, TEXT("Study_Window_East_0"));
	AddEastWall(3, 4, W_Wall, TEXT("Study_Wall_East_1"));

	// North Wall:
	AddNorthWall(2, 4, W_Wall, TEXT("Study_Wall_North_0"));
	AddNorthWall(3, 4, W_Window, TEXT("Study_Window_North_1"));

	// West Wall (Partition):
	AddWestWall(2, 3, W_Wall, TEXT("Study_Wall_West_Partition"));

	// Study Columns:
	AddColumn(4, 3, TEXT("Column_Study_SE"));
	AddColumn(4, 4, TEXT("Column_Study_E"));
	AddColumn(4, 5, TEXT("Column_Study_NE"));
	AddColumn(3, 5, TEXT("Column_Study_N"));

	// =========================================================================
	// 5. ROOM 3: 3-DOOR RIDDLE CHAMBER
	// =========================================================================
	// South Wall (From corridor):
	AddSouthWall(0, 6, W_Wall, TEXT("Chamber_Wall_South_Left"));
	AddSouthWall(2, 6, W_Wall, TEXT("Chamber_Wall_South_Right"));

	// West Wall:
	AddWestWall(0, 6, W_Wall, TEXT("Chamber_Wall_West_0"));
	AddWestWall(0, 7, W_Wall, TEXT("Chamber_Wall_West_1"));

	// East Wall:
	AddEastWall(2, 6, W_Wall, TEXT("Chamber_Wall_East_0"));
	AddEastWall(2, 7, W_Wall, TEXT("Chamber_Wall_East_1"));

	// North Wall (The 3 Riddle Doors in Archways):
	AddNorthWall(0, 7, W_Arch, TEXT("Chamber_Arch_Door_Left"));
	AddNorthWall(1, 7, W_Arch, TEXT("Chamber_Arch_Door_Middle"));
	AddNorthWall(2, 7, W_Arch, TEXT("Chamber_Arch_Door_Right"));

	// Dividing Columns between Riddle Doors:
	AddColumn(0, 6, TEXT("Column_Chamber_SW"));
	AddColumn(3, 6, TEXT("Column_Chamber_SE"));
	AddColumn(0, 8, TEXT("Column_Chamber_NW"));
	AddColumn(1, 8, TEXT("Column_Chamber_DoorDiv_1"));
	AddColumn(2, 8, TEXT("Column_Chamber_DoorDiv_2"));
	AddColumn(3, 8, TEXT("Column_Chamber_NE"));

	// Vestibule Enclosure Walls behind Riddle Doors (gy = 8):
	AddNorthWall(0, 8, W_Wall, TEXT("Vestibule_Backing_Wall_0"));
	AddNorthWall(1, 8, W_Wall, TEXT("Vestibule_Backing_Wall_1"));
	AddNorthWall(2, 8, W_Wall, TEXT("Vestibule_Backing_Wall_2"));
	AddWestWall(0, 8, W_Wall, TEXT("Vestibule_Wall_West"));
	AddEastWall(2, 8, W_Wall, TEXT("Vestibule_Wall_East"));

	// =========================================================================
	// 6. EXTERIOR FACADE SHELL (Prevents light leaks and covers bare backfaces)
	// =========================================================================
	if (W_ExteriorLightBlock)
	{
		// Backing exterior walls around the West and East exterior boundaries
		for (int32 gy = 0; gy <= 7; ++gy)
		{
			SpawnStaticMesh(W_ExteriorLightBlock, FVector(-10.0f, gy * 300.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Exterior/Facade"), FString::Printf(TEXT("Facade_West_%d"), gy));
		}
		for (int32 gy = 0; gy <= 7; ++gy)
		{
			SpawnStaticMesh(W_ExteriorLightBlock, FVector(910.0f, gy * 300.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f), FName("Mansion/Exterior/Facade"), FString::Printf(TEXT("Facade_East_%d"), gy));
		}
	}

	// =========================================================================
	// 7. PROFICIENT INTERIOR SET-DRESSING (Furniture, Books, Carpets, Clutter)
	// =========================================================================
	UStaticMesh* M_Desk = DeskMesh ? DeskMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Table_01a.SM_Table_01a"));
	UStaticMesh* M_SideTable = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Table_01b.SM_Table_01b"));
	UStaticMesh* M_Sofa = SofaMesh ? SofaMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Sofa_2M_01a.SM_Sofa_2M_01a"));
	UStaticMesh* M_Armchair = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Sofa_1M_01b.SM_Sofa_1M_01b"));
	UStaticMesh* M_Bookcase = BookcaseMesh ? BookcaseMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Wall_Bookcase_01a.SM_Wall_Bookcase_01a"));
	UStaticMesh* M_Carpet = CarpetMesh ? CarpetMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Carpet_01a.SM_Carpet_01a"));
	UStaticMesh* M_FireplaceDecor = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Fireplace_Deor_01a.SM_Fireplace_Deor_01a"));
	UStaticMesh* M_Notebook = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_OpenNotebook_01a.SM_OpenNotebook_01a"));
	UStaticMesh* M_Papers = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Papers_Group.SM_Papers_Group"));
	UStaticMesh* M_Books = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Book_Merged_01a.SM_Book_Merged_01a"));
	UStaticMesh* M_Curtains = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Curtain_01a.SM_Curtain_01a"));
	UStaticMesh* M_Chandelier = ChandelierMesh ? ChandelierMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Lamp_Top_01d.SM_Lamp_Top_01d"));
	UStaticMesh* M_WallLamp = WallLampMesh ? WallLampMesh.Get() : LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Lamp_Wall_01e.SM_Lamp_Wall_01e"));
	UStaticMesh* M_DeskLamp = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LivingRoom/Meshes/SM_Lamp_Small_01a.SM_Lamp_Small_01a"));

	// --- FOYER DRESSING ---
	// Large red runner carpet down the center aisle:
	SpawnStaticMesh(M_Carpet, FVector(450.0f, 450.0f, 1.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Foyer_Runner_Carpet"), FVector(1.2f, 2.5f, 1.0f));

	// Fireplace hearth tools:
	SpawnStaticMesh(M_FireplaceDecor, FVector(240.0f, 850.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Foyer_Fireplace_Tools"));

	// Hanging Chandelier:
	SpawnStaticMesh(M_Chandelier, FVector(450.0f, 450.0f, 250.0f), FRotator::ZeroRotator, FName("Mansion/Lighting"), TEXT("Foyer_Chandelier_Mesh"));

	// East Wall Seating & Key Table:
	SpawnStaticMesh(M_Sofa, FVector(750.0f, 450.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Foyer_Victorian_Sofa"));
	SpawnStaticMesh(M_SideTable, FVector(750.0f, 150.0f, 0.0f), FRotator::ZeroRotator, FName("Mansion/Furniture"), TEXT("Foyer_Key_Table"));
	SpawnStaticMesh(M_Armchair, FVector(200.0f, 700.0f, 0.0f), FRotator(0.0f, -45.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Foyer_Hearth_Armchair"));

	// Window Curtains in Foyer:
	SpawnStaticMesh(M_Curtains, FVector(15.0f, 150.0f, 140.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Foyer_Curtains_0"));
	SpawnStaticMesh(M_Curtains, FVector(15.0f, 750.0f, 140.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Foyer_Curtains_1"));

	// --- CORRIDOR DRESSING ---
	// Bookcases lining the west wall:
	SpawnStaticMesh(M_Bookcase, FVector(320.0f, 1050.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Corridor_Bookcase_0"));
	SpawnStaticMesh(M_Bookcase, FVector(320.0f, 1650.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Corridor_Bookcase_1"));
	SpawnStaticMesh(M_Books, FVector(330.0f, 1050.0f, 85.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Corridor_Books_Stack"));

	// Wall Sconces in corridor:
	SpawnStaticMesh(M_WallLamp, FVector(320.0f, 1200.0f, 210.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Lighting"), TEXT("Corridor_Sconce_0"));
	SpawnStaticMesh(M_WallLamp, FVector(320.0f, 1500.0f, 210.0f), FRotator(0.0f, 90.0f, 0.0f), FName("Mansion/Lighting"), TEXT("Corridor_Sconce_1"));

	// --- VICTOR'S STUDY DRESSING ---
	// Large mahogany executive desk:
	SpawnStaticMesh(M_Desk, FVector(900.0f, 1200.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Victor_Desk"));
	SpawnStaticMesh(M_Armchair, FVector(900.0f, 1300.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Desk_Chair"));

	// Desk Clutter (Notebook, loose investigation papers, banker's lamp):
	SpawnStaticMesh(M_Notebook, FVector(870.0f, 1180.0f, 76.0f), FRotator(0.0f, 15.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Open_Notebook"));
	SpawnStaticMesh(M_Papers, FVector(940.0f, 1180.0f, 75.0f), FRotator(0.0f, -20.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Loose_Papers"));
	SpawnStaticMesh(M_DeskLamp, FVector(820.0f, 1220.0f, 75.0f), FRotator(0.0f, 45.0f, 0.0f), FName("Mansion/Lighting"), TEXT("Study_Desk_Lamp_Mesh"));

	// Study Bookcases:
	SpawnStaticMesh(M_Bookcase, FVector(1050.0f, 1480.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Bookcase_North"));
	SpawnStaticMesh(M_Bookcase, FVector(750.0f, 920.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Bookcase_South"));
	SpawnStaticMesh(M_Sofa, FVector(1100.0f, 1050.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Leather_Sofa"));

	// Window Curtains in Study:
	SpawnStaticMesh(M_Curtains, FVector(1185.0f, 1050.0f, 140.0f), FRotator(0.0f, -90.0f, 0.0f), FName("Mansion/Furniture"), TEXT("Study_Window_Curtains"));

	// --- OUTDOOR PROPS (Barrels & Crates by Porch) ---
	UStaticMesh* M_Barrel = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Props/SM_Barell_01.SM_Barell_01"));
	UStaticMesh* M_Box = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/MafiaBarnPack/Meshes/Props/SM_Box_01.SM_Box_01"));
	if (M_Barrel)
	{
		SpawnStaticMesh(M_Barrel, FVector(200.0f, -150.0f, -10.0f), FRotator::ZeroRotator, FName("Mansion/Exterior/Props"), TEXT("Courtyard_Barrel_0"));
		SpawnStaticMesh(M_Barrel, FVector(700.0f, -160.0f, -10.0f), FRotator::ZeroRotator, FName("Mansion/Exterior/Props"), TEXT("Courtyard_Barrel_1"));
	}
	if (M_Box)
	{
		SpawnStaticMesh(M_Box, FVector(720.0f, -110.0f, -10.0f), FRotator(0.0f, 25.0f, 0.0f), FName("Mansion/Exterior/Props"), TEXT("Courtyard_Crate_0"));
	}
}

void AMansionBuilder::BuildLighting()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawns an editable APointLight actor in the level
	auto SpawnLight = [&](const FVector& RelLoc, FLinearColor Color, float Intensity, float Radius, const FString& Label) -> APointLight*
	{
		APointLight* LightActor = World->SpawnActor<APointLight>(Origin + RelLoc, FRotator::ZeroRotator, SpawnParams);
		if (LightActor)
		{
			UPointLightComponent* PLC = Cast<UPointLightComponent>(LightActor->GetLightComponent());
			if (PLC)
			{
				PLC->SetLightColor(Color);
				PLC->Intensity = Intensity;
				PLC->AttenuationRadius = Radius;
				PLC->bUseInverseSquaredFalloff = true;
				PLC->SetMobility(EComponentMobility::Movable);
			}
			LightActor->SetActorLabel(Label);
			LightActor->SetFolderPath(FName("Mansion/Lighting"));
			SpawnedDoorsAndProps.Add(LightActor);
		}
		return LightActor;
	};

	// 1. Exterior Porch Entrance Lantern (Amber cutting into night fog):
	SpawnLight(FVector(450.0f, -60.0f, 240.0f), FLinearColor(1.0f, 0.70f, 0.35f), 1800.0f, 500.0f, TEXT("Light_Exterior_Porch_Lantern"));

	// 2. Foyer Grand Chandelier (Warm Amber 3500K):
	SpawnLight(FVector(450.0f, 450.0f, 230.0f), FLinearColor(1.0f, 0.75f, 0.45f), 2800.0f, 750.0f, TEXT("Light_Foyer_Chandelier"));

	// 3. Foyer Fireplace Ember Glow:
	SpawnLight(FVector(150.0f, 850.0f, 50.0f), FLinearColor(1.0f, 0.40f, 0.08f), 1200.0f, 350.0f, TEXT("Light_Foyer_Fireplace"));

	// 4. Corridor Flickering Wall Sconces:
	SpawnLight(FVector(340.0f, 1200.0f, 210.0f), FLinearColor(0.85f, 0.65f, 0.45f), 650.0f, 350.0f, TEXT("Light_Corridor_Sconce_0"));
	SpawnLight(FVector(340.0f, 1500.0f, 210.0f), FLinearColor(0.85f, 0.65f, 0.45f), 650.0f, 350.0f, TEXT("Light_Corridor_Sconce_1"));

	// 5. Victor's Study Desk Reading Lamp:
	SpawnLight(FVector(830.0f, 1210.0f, 110.0f), FLinearColor(1.0f, 0.70f, 0.30f), 1500.0f, 450.0f, TEXT("Light_Study_Desk_Lamp"));

	// 6. 3-Door Riddle Chamber: Supernatural Cold Pale Cyan Rim Lighting:
	SpawnLight(FVector(450.0f, 2100.0f, 260.0f), FLinearColor(0.35f, 0.75f, 1.0f), 2200.0f, 750.0f, TEXT("Light_RiddleChamber_Supernatural"));
	SpawnLight(FVector(150.0f, 2380.0f, 240.0f), FLinearColor(0.30f, 0.70f, 1.0f), 800.0f, 250.0f, TEXT("Light_Chamber_DoorLeft"));
	SpawnLight(FVector(450.0f, 2380.0f, 240.0f), FLinearColor(0.80f, 0.20f, 0.1f), 800.0f, 250.0f, TEXT("Light_Chamber_DoorMiddle_Trap"));
	SpawnLight(FVector(750.0f, 2380.0f, 240.0f), FLinearColor(0.30f, 0.70f, 1.0f), 800.0f, 250.0f, TEXT("Light_Chamber_DoorRight"));
}

void AMansionBuilder::BuildGameplayActors()
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 1. Mila's Portrait in Foyer (Mounted on mantelpiece above fireplace at gx = 0, gy = 2)
	const FVector PortraitLoc = Origin + FVector(150.0f, 885.0f, 160.0f);
	const FRotator PortraitRot = FRotator(0.0f, 180.0f, 0.0f);
	SpawnedPortrait = World->SpawnActor<AHorrorPortraitClue>(PortraitLoc, PortraitRot, SpawnParams);
	if (SpawnedPortrait)
	{
		SpawnedPortrait->SetActorLabel(TEXT("Gameplay_MilaPortraitClue"));
		SpawnedPortrait->SetFolderPath(FName("Mansion/Gameplay"));
	}

	// 2. Study Key on Side Table in Foyer (gx = 2, gy = 0 at Z = 75)
	const FVector KeyLoc = Origin + FVector(750.0f, 150.0f, 75.0f);
	SpawnedKey = World->SpawnActor<AHorrorKeyPickup>(KeyLoc, FRotator::ZeroRotator, SpawnParams);
	if (SpawnedKey)
	{
		SpawnedKey->SetActorLabel(TEXT("Gameplay_StudyKey_Pickup"));
		SpawnedKey->SetFolderPath(FName("Mansion/Gameplay"));
	}

	// 3. Locked Study Door (East wall of corridor, gy = 4, X = 600)
	const FVector StudyDoorLoc = Origin + FVector(600.0f, 1350.0f, 0.0f);
	const FRotator StudyDoorRot = FRotator(0.0f, 90.0f, 0.0f);
	SpawnedStudyDoor = World->SpawnActor<AHorrorDoor>(StudyDoorLoc, StudyDoorRot, SpawnParams);
	if (SpawnedStudyDoor)
	{
		SpawnedStudyDoor->LockDoor(FName("Key_Study"));
		SpawnedStudyDoor->SetActorLabel(TEXT("Gameplay_StudyDoor_Locked"));
		SpawnedStudyDoor->SetFolderPath(FName("Mansion/Gameplay"));
		SpawnedDoorsAndProps.Add(SpawnedStudyDoor);
	}

	// 4. Stopped Grandfather Clock in Study (North wall of study, gy = 4, X = 750)
	const FVector ClockLoc = Origin + FVector(750.0f, 1480.0f, 0.0f);
	const FRotator ClockRot = FRotator(0.0f, 180.0f, 0.0f);
	SpawnedClock = World->SpawnActor<AGrandfatherClock>(ClockLoc, ClockRot, SpawnParams);
	if (SpawnedClock)
	{
		SpawnedClock->SetActorLabel(TEXT("Gameplay_GrandfatherClock"));
		SpawnedClock->SetFolderPath(FName("Mansion/Gameplay"));
		SpawnedDoorsAndProps.Add(SpawnedClock);
	}

	// 5. 3-Door Puzzle Room Manager in Riddle Chamber
	const FVector PuzzleRoomLoc = Origin + FVector(450.0f, 2100.0f, 0.0f);
	SpawnedPuzzleRoom = World->SpawnActor<AThreeDoorPuzzleRoom>(PuzzleRoomLoc, FRotator::ZeroRotator, SpawnParams);
	if (SpawnedPuzzleRoom)
	{
		SpawnedPuzzleRoom->SetActorLabel(TEXT("Gameplay_PuzzleRoomManager"));
		SpawnedPuzzleRoom->SetFolderPath(FName("Mansion/Gameplay"));
		SpawnedDoorsAndProps.Add(SpawnedPuzzleRoom);
	}

	// Spawn the 3 riddle doors along the North wall of Riddle Chamber (gy = 7, Y = 2400)
	auto SpawnRiddleDoor = [&](float XCoord, bool bIsLocked, const FName& KeyId, const FString& Label) -> AHorrorDoor*
	{
		const FVector DoorLoc = Origin + FVector(XCoord, 2400.0f, 0.0f);
		const FRotator DoorRot = FRotator(0.0f, 180.0f, 0.0f);
		AHorrorDoor* Door = World->SpawnActor<AHorrorDoor>(DoorLoc, DoorRot, SpawnParams);
		if (Door)
		{
			if (bIsLocked)
			{
				Door->LockDoor(KeyId);
			}
			Door->SetActorLabel(Label);
			Door->SetFolderPath(FName("Mansion/Gameplay"));
			SpawnedDoorsAndProps.Add(Door);
		}
		return Door;
	};

	// Left Door = True Escape! (gx = 0, center at X = 150)
	AHorrorDoor* LeftDoor = SpawnRiddleDoor(150.0f, false, NAME_None, TEXT("Door_Riddle_0_Escape"));

	// Middle Door = Deadly Trap! (gx = 1, center at X = 450)
	AHorrorDoor* MiddleDoor = SpawnRiddleDoor(450.0f, false, NAME_None, TEXT("Door_Riddle_1_Trap"));

	// Right Door = Barred Dead End! (gx = 2, center at X = 750)
	AHorrorDoor* RightDoor = SpawnRiddleDoor(750.0f, true, FName("Key_Barred"), TEXT("Door_Riddle_2_Barred"));

	// Connect doors to the puzzle room logic
	if (SpawnedPuzzleRoom)
	{
		SpawnedPuzzleRoom->ConfigureDoors(LeftDoor, MiddleDoor, RightDoor);
	}
}
