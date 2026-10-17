# Flashlight Mechanic Implementation: In-Depth Technical Report

**Project**: *Did You Know?* (Horror Mansion Prototype)  
**Engine Version**: Unreal Engine 5.8  
**Repository Branch**: `main` (Latest Remote Commit: `0d22da2`)  
**Document Purpose**: Definitive technical specification and progress report of the first-person flashlight mechanic, including C++ architecture, Animation Blueprint integration, asset dependency audit, recent bug fixes, and target visual adjustments for subsequent pair-programming sessions.

---

## 1. Executive Summary & Architecture Overview

The flashlight mechanic is engineered as a hybrid **C++ Core + First-Person AnimBlueprint** system designed for high responsiveness, zero input lag, and realistic visual immersion:

1. **C++ Core ([`AHorrorCharacter`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Source/DidYouKnow/Variant_Horror/HorrorCharacter.h))**:
   - Manages the physical flashlight skeletal mesh, spotlight component, dual-source input polling (Enhanced Input + hardware fallback), audio playback, and smooth arm-raise interpolation.
   - Pushes state directly to the first-person Animation Instance (`ABP_FP_Copy`) via reflection every frame, eliminating Blueprint VM execution overhead and preventing missing-symbol compiler crashes.
2. **Animation Blueprint ([`ABP_FP_Copy`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Content/FirstPerson/Anims/ABP_FP_Copy.uasset))**:
   - Copies base full-body locomotion from the parent third-person mannequin (`SKM_Manny`) using `Copy Pose From Mesh`.
   - Evaluates a 2D Aim Offset ([`OFSSET_FlashLight.uasset`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Content/FlashLight_System/Animations/FlashLightAnimation/OFSSET_FlashLight.uasset)) driven by camera pitch and yaw.
   - Dampens pitch and yaw to prevent extreme weapon clipping during vertical and horizontal camera sweeps.
   - Blends the additive arm pose smoothly using `FlashlightAlpha`.
3. **Curated Asset Suite ([`Content/FlashLight_System/`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Content/FlashLight_System))**:
   - Project-wide byte-level dependency scan confirmed **exactly 15 essential assets** are actively used.
   - **49 obsolete third-person marketplace template files** were cleanly purged from disk and Git.

```
                    ┌────────────────────────────────────────┐
                    │       Player Input (F Key Pressed)     │
                    └───────────────────┬────────────────────┘
                                        │
                                        ▼
             ┌─────────────────────────────────────────────────────┐
             │            AHorrorCharacter (C++ Core)             │
             │  - Toggles bFlashlightOn                            │
             │  - Plays 3D Sound (Flashlight_On / Flashlight_OFF)   │
             │  - Toggles SpotLight (12,000 Lumens)                │
             │  - Smoothly interps CurrentFlashlightAlpha [0..1]   │
             │  - Pushes properties directly to AnimInstance:      │
             │      * FlashlightAlpha                              │
             │      * FlashlightPoseActive                         │
             │      * FlashlightAimY (Pitch)                       │
             │      * FlashlightAimX (Yaw)                         │
             └──────────────────────────┬──────────────────────────┘
                                        │
                                        ▼
             ┌─────────────────────────────────────────────────────┐
             │              ABP_FP_Copy (AnimGraph)                │
             │  - Base Locomotion: Copy Pose From Mesh             │
             │  - Aim Offset: OFSSET_FlashLight                    │
             │  - Additive Blend: Driven by FlashlightAlpha        │
             │  - Output: Smooth first-person flashlight pose      │
             └─────────────────────────────────────────────────────┘
```

---

## 2. C++ Gameplay Implementation Details

**Header**: [`HorrorCharacter.h`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Source/DidYouKnow/Variant_Horror/HorrorCharacter.h)  
**Source**: [`HorrorCharacter.cpp`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Source/DidYouKnow/Variant_Horror/HorrorCharacter.cpp)

### A. Mesh & Component Setup
* **`FlashlightMesh` (`UStaticMeshComponent*`)**:
  * Asset path: `/Game/FlashLight_System/Mesh/SK_Flashlight.SK_Flashlight`
  * Attach Socket: Snaps to socket **`FlashLight`** on `GetFirstPersonMesh()` (`SKM_Manny_Simple`).
  * Transformation rules: `FAttachmentTransformRules::SnapToTargetNotIncludingScale`.
  * Relative transform: Location `(0, 0, 0)`, Rotation `(0, 0, 0)`, Scale `(1.0, 1.0, 1.0)`.
  * Collision & Shadow: `SetCollisionProfileName("NoCollision")`, `SetCastShadow(false)`, `FirstPersonPrimitiveType = FirstPerson`, `SetOnlyOwnerSee(true)`.
  * Initial State: Starts hidden (`SetVisibility(false)`).

* **`SpotLight` (`USpotLightComponent*`)**:
  * Attached directly to `FlashlightMesh`.
  * Relative Location: `SpotLightRelativeLocation = FVector(0.0f, 0.0f, 16.0f)` *(aligned exactly at the front glass lens)*.
  * Relative Rotation: `FRotator(90.0f, 0.0f, 0.0f)` *(coaligned with the flashlight cylinder forward axis)*.
  * Intensity: `12000.0f` lumens (`ELightUnits::Lumens`).
  * Attenuation Radius: `3500.0f` cm (~35 meters).
  * Cone Angles: Inner `18.0f` degrees, Outer `36.0f` degrees.
  * Initial State: Disabled (`SetVisibility(false)`).

### B. Audio Integration
Four sound assets are referenced and loaded via `ConstructorHelpers::FObjectFinder`:
1. `FlashlightOnSound`: `/Game/FlashLight_System/Sound/Flashlight_On.Flashlight_On`
2. `FlashlightOffSound`: `/Game/FlashLight_System/Sound/Flashlight_OFF.Flashlight_OFF`
3. `FlashlightAttenuation`: `/Game/FlashLight_System/Sound/Att_FlashLight.Att_FlashLight`
4. `LightFlickerSound`: `/Game/FlashLight_System/Sound/LightFlicker02.LightFlicker02`

Audio is played using `UGameplayStatics::PlaySoundAtLocation` at `GetActorLocation()` using `FlashlightAttenuation` for realistic 3D spatial roll-off.

### C. Input Handling & Debouncing
Flashlight toggling is bound to the **F** key with dual redundancy:
1. **Enhanced Input**: Bound to `FlashlightAction` (`ETriggerEvent::Started`).
2. **Direct Hardware Polling**: In `Tick()`, `PC->WasInputKeyJustPressed(EKeys::F)` guarantees responsiveness even during mapping context transitions.
3. **Debounce Guard**: 250ms threshold prevents double-triggering on key bounce:
   ```cpp
   const double CurrentTime = FPlatformTime::Seconds();
   if (CurrentTime - GLastFlashlightToggleTime < 0.25) return;
   GLastFlashlightToggleTime = CurrentTime;
   ```

### D. Arm Raise / Lower Interpolation
In [`AHorrorCharacter::Tick()`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Source/DidYouKnow/Variant_Horror/HorrorCharacter.cpp#L172-L175):
* `TargetAlpha = bFlashlightOn ? 1.0f : 0.0f;`
* `CurrentFlashlightAlpha = FMath::FInterpTo(CurrentFlashlightAlpha, TargetAlpha, DeltaTime, FlashlightRaiseInterpSpeed);`
* `FlashlightRaiseInterpSpeed = 8.0f;`
* Mesh visibility persistence: `const bool bShouldShowMesh = bFlashlightOn || (CurrentFlashlightAlpha > 0.02f);` ensures the mesh stays visible while lowering until completely at rest.

### E. Animation Property Push (Tick Synchronization)
Rather than forcing Blueprint to cast every frame, C++ reflects and sets variables directly on the first-person `UAnimInstance`:
```cpp
SetNumericProp(TEXT("FlashlightAlpha"), CurrentFlashlightAlpha);
ActiveProp->SetPropertyValue_InContainer(AnimInst, bFlashlightOn);
SetNumericProp(TEXT("FlashlightAimY"), GetFlashlightAimPitch());
SetNumericProp(TEXT("FlashlightAimX"), 0.0);
```

---

## 3. Animation Blueprint Implementation

**Asset**: [`ABP_FP_Copy.uasset`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Content/FirstPerson/Anims/ABP_FP_Copy.uasset)

### A. EventGraph Logic & Dampening
In `EventBlueprintUpdateAnimation`, relative camera aim rotation is computed and dampened:
1. `DeltaRot = Math::Rotator::Delta(GetBaseAimRotation(TryGetPawnOwner), GetActorRotation(TryGetPawnOwner))`
2. **Pitch Dampening (`FlashlightAimY`)**:
   $$\text{Pitch} = \text{Clamp}(\text{DeltaRot.Pitch} \times 0.45, -35.0^\circ, 30.0^\circ)$$
   *Multiplied by 0.45 (55% reduction) and clamped between -35° and +30° to prevent the arm from driving the flashlight mesh into the camera viewport when looking upward.*
3. **Yaw Dampening (`FlashlightAimX`)**:
   $$\text{Yaw} = \text{Clamp}(\text{DeltaRot.Yaw} \times 0.35, -25.0^\circ, 25.0^\circ)$$
   *Multiplied by 0.35 and clamped to [-25°, 25°] for natural wrist alignment during horizontal turns.*

### B. AnimGraph Evaluation
1. **Base Pose**: `Copy Pose From Mesh` copies locomotion from the parent character mesh (`SKM_Manny`).
2. **Aim Offset Node**: Evaluates [`OFSSET_FlashLight.uasset`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Content/FlashLight_System/Animations/FlashLightAnimation/OFSSET_FlashLight.uasset) with inputs `FlashlightAimX` (Yaw) and `FlashlightAimY` (Pitch).
3. **Additive Blending**: Uses `Apply Additive` weighted by `FlashlightAlpha`.
   - `FlashlightAlpha = 0.0`: Unarmed relaxed idle/walk/run arms.
   - `FlashlightAlpha = 1.0`: Smoothly raised left arm holding the illuminated flashlight.

---

## 4. Curated Asset Registry (The 15 Kept Files)

All 15 essential assets reside under [`Content/FlashLight_System/`](file:///c:/Users/moham/unreal_project/Did_You_know/DidYouKnow/Content/FlashLight_System):

| Category | File Name | Asset Type | Functional Role |
| :--- | :--- | :--- | :--- |
| **Aim Offset** | `OFSSET_FlashLight.uasset` | Aim Offset 2D | Evaluated in `ABP_FP_Copy` AnimGraph |
| **Aim Poses** | `MM_FlashLight_Idle.uasset` | Anim Sequence | Additive center forward aim pose |
| **Aim Poses** | `MM_FlashLight_-90.uasset` | Anim Sequence | Additive 90° left aim pose |
| **Aim Poses** | `MM_FlashLight_90.uasset` | Anim Sequence | Additive 90° right aim pose |
| **Aim Poses** | `MM_FlashLight_Up90.uasset` | Anim Sequence | Additive 90° upward aim pose |
| **Aim Poses** | `MM_FlashLight_Down90.uasset` | Anim Sequence | Additive 90° downward aim pose |
| **Aim Poses** | `MM_FlashLight_Additive.uasset` | Anim Sequence | Base additive reference frame |
| **Mesh** | `SK_Flashlight.uasset` | Skeletal Mesh | Physical 3D flashlight attached to hand |
| **Materials** | `MI_Flashlight_Body.uasset` | Material Instance | Flashlight metallic housing shader |
| **Materials** | `M_Glass_Flashlight.uasset` | Material | Translucent front lens glass shader |
| **Textures** | `T_flashlight.uasset` | Texture2D | Body diffuse & roughness texture |
| **Audio** | `Flashlight_On.uasset` | SoundWave | Click ON sound effect |
| **Audio** | `Flashlight_OFF.uasset` | SoundWave | Click OFF sound effect |
| **Audio** | `Att_FlashLight.uasset` | SoundAttenuation | 3D spatial roll-off configuration |
| **Audio** | `LightFlicker02.uasset` | SoundWave | Electrical stutter/flicker SFX |

### Purged Junk Files (49 Deleted)
* **22 Locomotion Animations**: `ALS_N_Walk_*`, `Run_*`, `Walk_*`, `MM_Unarmed_Jog_*` *(third-person marketplace template locomotion; unused)*.
* **6 Obsolete Blueprints**: `BP_Flashlight`, `BP_Fail_Flashlight`, `BPC_Interact`, `BPI_Interact`, `BP_Master_Interact`, `S_Item` *(old marketplace BP actors; logic replaced by native C++)*.
* **5 Demo Grid Assets**: `MF_ProcGrid`, `M_PrototypeGrid`, `MI_PrototypeGrid_TopDark*`, `T_GridChecker_A` *(showcase level materials; unused)*.
* **7 Input Actions**: `IA_FlashLIght`, `IA_Interact`, `IA_Jump`, `IA_Look`, `IA_Move`, `IA_Run`, `IMC_Default` *(marketplace input mapping; game uses `IMC_Horror`)*.
* **9 UI & Promo Assets**: `WB_HudPlayer`, `WB_Interact`, `M_Screenshot_*`, `T_Screenshot_*`, `T_Icon_*`, `spooky-horror-ambience` *(marketplace promo images/widgets)*.

---

## 5. Recent Fixes Applied

### A. Resolution of the `ABP_FP_Copy` Compiler Error
* **Symptom**: `ABP_FP_Copy` showed compile errors:
  * *"Could not find a function named 'IsFlashlightOn' in 'HorrorCharacter'"*
  * *"Could not find a function named 'GetFlashlightAlpha' in 'HorrorCharacter'"*
* **Root Cause**:
  `ABP_FP_Copy:EventGraph` contained redundant Blueprint nodes calling `GetFlashlightAlpha` and `IsFlashlightOn`. When the editor launched fresh from disk, the base binary DLL (`UnrealEditor-DidYouKnow.dll`) had not yet registered the new UFUNCTION metadata without Live Coding being manually triggered.
* **Fix Applied**:
  Updated `ABP_FP_Copy:EventGraph` to remove the redundant function calls. The AnimInstance already receives `FlashlightAlpha` and `FlashlightPoseActive` directly from C++ `Tick()`. Recompiled and saved `ABP_FP_Copy.uasset` with **zero errors (green checkmark)**.

---

## 6. Targets for Next Chat Adjustments

When opening the new chat session, focus on these visual refinement items:

1. **Upward Look Viewport Clearance**:
   - *Current Behavior*: Looking upward brings the flashlight cylinder close to the first-person camera lens.
   - *Formula to Adjust*: In `ABP_FP_Copy:EventGraph`, adjust the max pitch clamp:
     ```
     (ClampAngle (* (.pitch DeltaRot) 0.45) -35.0 15.0)  // Reduced upper clamp from 30.0 to 15.0
     ```
   - *Alternative*: Nudge the `FlashLight` socket position forward or slightly down on `SKM_Manny_Simple`.
2. **Center-Left Hand/Wrist Alignment**:
   - *Current Behavior*: Panning horizontally center-left shows minor wrist angular distortion at wide FOVs.
   - *Formula to Adjust*: Fine-tune yaw multiplier in `EventGraph` from `0.35` to `0.25` or clamp range to `[-18.0, 18.0]`.
   - *Alternative*: Tweak `FlashlightGripRotation` in `AHorrorCharacter` if overriding socket rotation.
3. **C++ Project Rebuild**:
   - Run a clean build via Visual Studio or UnrealBuildTool while Unreal Editor is closed to update `Binaries/Win64/UnrealEditor-DidYouKnow.dll` permanently so all new C++ reflection properties are baked into the disk binary.
