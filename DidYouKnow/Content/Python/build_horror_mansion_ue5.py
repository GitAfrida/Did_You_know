"""
================================================================================
UNREAL ENGINE 5.8.3 — "DID YOU KNOW?" (V8 AAA DEFINITIVE EDITION)
================================================================================
File: build_horror_mansion_ue5.py
Game: DID YOU KNOW? — Graves Country House (Victor, Mara & Mila Graves)
Protagonist: Cross Grover (UK University Criminology Student)
Compatibility: Unreal Engine 5.8.3 (100% Verified Against `My_UE5_Project_Assets.txt`)
================================================================================
"""

import math
import os
import sys
import types

try:
    import unreal
    IN_UNREAL_EDITOR = True
except ImportError:
    IN_UNREAL_EDITOR = False


# ==============================================================================
# 1. VERIFIED ASSET PATHS FROM `My_UE5_Project_Assets.txt`
#    (ZERO `MafiaBarnPack`, ZERO `SM_Env_01a/b`, ZERO `M_Rock_Slate`, ZERO `SM_Chair`)
# ==============================================================================

MAT_PATHS = {
    "ground_moss": "/Game/StarterContent/Materials/M_Ground_Moss",
    "ground_gravel": "/Game/StarterContent/Materials/M_Ground_Gravel",
    "cobble_rough": "/Game/StarterContent/Materials/M_CobbleStone_Rough",
    "cobble_smooth": "/Game/StarterContent/Materials/M_CobbleStone_Smooth",
    "stone_hewn": "/Game/StarterContent/Materials/M_Brick_Cut_Stone",
    "stone_cut": "/Game/StarterContent/Materials/M_Brick_Cut_Stone",
    "rock_basalt": "/Game/StarterContent/Materials/M_Rock_Basalt",
    "rock_slate": "/Game/StarterContent/Materials/M_Rock_Basalt",
    "marble_polished": "/Game/StarterContent/Materials/M_Rock_Marble_Polished",
    "brick_old": "/Game/StarterContent/Materials/M_Brick_Clay_Old",
    "brick_beveled": "/Game/StarterContent/Materials/M_Brick_Clay_Beveled",
    "wood_walnut_polished": "/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished",
    "wood_walnut_worn": "/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Worn",
    "wood_walnut": "/Game/StarterContent/Materials/M_Wood_Walnut",
    "wood_oak": "/Game/StarterContent/Materials/M_Wood_Oak",
    "wood_pine": "/Game/StarterContent/Materials/M_Wood_Pine",
    "tile_checker": "/Game/StarterContent/Materials/M_Ceramic_Tile_Checker",
    "concrete_tiles": "/Game/StarterContent/Materials/M_Concrete_Tiles",
    "concrete_grime": "/Game/StarterContent/Materials/M_Concrete_Grime",
    "concrete_poured": "/Game/StarterContent/Materials/M_Concrete_Poured",
    "metal_rust": "/Game/StarterContent/Materials/M_Metal_Rust",
    "metal_steel": "/Game/StarterContent/Materials/M_Metal_Burnished_Steel",
    "metal_gold": "/Game/StarterContent/Materials/M_Metal_Gold",
    "metal_copper": "/Game/StarterContent/Materials/M_Rock_Basalt",
    "glass": "/Game/StarterContent/Materials/M_Glass",
    "water_lake": "/Game/StarterContent/Materials/M_Water_Lake",
    "leartes_floor": "/Game/LivingRoom/Material/MI_BLD_Floor_3M_01a",
    "leartes_tile1": "/Game/LivingRoom/Material/MI_Tile_01",
    "leartes_tile4": "/Game/LivingRoom/Material/MI_Tile_04",
    "leartes_trim1": "/Game/LivingRoom/Material/MI_Trim_01",
    "leartes_trim2": "/Game/LivingRoom/Material/MI_Trim_02",
    "decal_footprint": "/Game/LivingRoom/Material/MI_D_Footprint",
    "decal_grunge": "/Game/LivingRoom/Material/MI_D_Grunge",
    "decal_coffee": "/Game/LivingRoom/Material/MI_D_Coffee_Stain",
    "decal_stain": "/Game/LivingRoom/Material/MI_D_Stain_01a",
    "decal_tablecloth_a": "/Game/LivingRoom/Material/MI_D_Tabecloth_01a",
    "decal_tablecloth_b": "/Game/LivingRoom/Material/MI_D_Tabecloth_01b",
    "decal_paper_a": "/Game/LivingRoom/Material/MI_D_Paper_01a",
    "decal_paper_b": "/Game/LivingRoom/Material/MI_D_Paper_01b",
    "decal_tape_a": "/Game/LivingRoom/Material/MI_D_Tape_01a",
}

STARTER_TEX_PATHS = {
    "ground_moss": ("/Game/StarterContent/Textures/T_Ground_Moss_D", "/Game/StarterContent/Textures/T_Ground_Moss_N"),
    "ground_gravel": ("/Game/StarterContent/Textures/T_Ground_Gravel_D", "/Game/StarterContent/Textures/T_Ground_Gravel_N"),
    "cobble_rough": ("/Game/StarterContent/Textures/T_CobbleStone_Rough_D", "/Game/StarterContent/Textures/T_CobbleStone_Rough_N"),
    "cobble_smooth": ("/Game/StarterContent/Textures/T_CobbleStone_Smooth_D", "/Game/StarterContent/Textures/T_CobbleStone_Smooth_N"),
    "stone_cut": ("/Game/StarterContent/Textures/T_Brick_Cut_Stone_D", "/Game/StarterContent/Textures/T_Brick_Cut_Stone_N"),
    "brick_old": ("/Game/StarterContent/Textures/T_Brick_Clay_Old_D", "/Game/StarterContent/Textures/T_Brick_Clay_Old_N"),
    "rock_basalt": ("/Game/StarterContent/Textures/T_Rock_Basalt_D", "/Game/StarterContent/Textures/T_Rock_Basalt_N"),
    "wood_walnut_polished": ("/Game/StarterContent/Textures/T_Wood_Floor_Walnut_D", "/Game/StarterContent/Textures/T_Wood_Floor_Walnut_N"),
    "wood_walnut_worn": ("/Game/StarterContent/Textures/T_Wood_Floor_Walnut_D", "/Game/StarterContent/Textures/T_Wood_Floor_Walnut_N"),
    "concrete_tiles": ("/Game/StarterContent/Textures/T_Concrete_Tiles_D", "/Game/StarterContent/Textures/T_Concrete_Tiles_N"),
    "concrete_grime": ("/Game/StarterContent/Textures/T_Concrete_Grime_D", None),
}

TILED_MAT_RECIPES = {
    "ground_moss_tiled": ("ground_moss", "M_Graves_ParklandMoss_V8", 35.0, 35.0, 0.85, None),
    "ground_gravel_tiled": ("ground_gravel", "M_Graves_Gravel_V8", 18.0, 18.0, 0.85, None),
    "cobble_road_tiled": ("cobble_rough", "M_Graves_CobbleRoad_V8", 2.5, 2.5, 0.45, "T_Graves_MossyCobble_D.jpg"),
    "cobble_drive_tiled": ("cobble_rough", "M_Graves_CobbleDrive_V8", 2.5, 2.5, 0.42, "T_Graves_MossyCobble_D.jpg"),
    "cobble_forecourt_tiled": ("cobble_smooth", "M_Graves_Forecourt_V8", 10.0, 10.0, 0.48, "T_Graves_MossyCobble_D.jpg"),
    "stone_hewn_tiled": ("stone_cut", "M_Graves_AshlarStone1F_V8", 3.0, 2.5, 0.75, "T_Graves_AshlarStone_D.jpg"),
    "stone_cut_tiled": ("stone_cut", "M_Graves_CutStoneTrim_V8", 2.5, 2.0, 0.72, "T_Graves_AshlarStone_D.jpg"),
    "brick_old_tiled": ("brick_old", "M_Graves_OldBrick2F_V8", 3.5, 2.5, 0.78, "T_Graves_OldBrick_D.jpg"),
    "rock_slate_roof_tiled": ("rock_basalt", "M_Graves_DarkSlateRoof_V8", 8.0, 4.0, 0.65, "T_Graves_DarkSlate_D.jpg"),
    "wood_floor_polished_tiled": ("wood_walnut_polished", "M_Graves_WoodPolished_V8", 6.0, 6.0, 0.32, None),
    "wood_floor_worn_tiled": ("wood_walnut_worn", "M_Graves_WoodWorn_V8", 6.0, 6.0, 0.58, None),
    "concrete_tiles_tiled": ("concrete_tiles", "M_Graves_BasementTiles_V8", 8.0, 8.0, 0.75, None),
    "concrete_grime_tiled": ("concrete_grime", "M_Graves_AgedPlaster_V8", 3.0, 2.0, 0.82, None),
}

LEARTES_MESHES = {
    "BLD_Wall_1M": "/Game/LivingRoom/Meshes/SM_BLD_Wall_1M",
    "BLD_Wall_3M_A": "/Game/LivingRoom/Meshes/SM_BLD_Wall_3M_01a",
    "BLD_Wall_3M_B": "/Game/LivingRoom/Meshes/SM_BLD_Wall_3M_01b",
    "BLD_Wall_4M": "/Game/LivingRoom/Meshes/SM_BLD_Wall_4M_02a",
    "BLD_Wall_Fireplace_2M": "/Game/LivingRoom/Meshes/SM_BLD_Wall_Fireplace_2M",
    "BLD_Wall_Inner_Room": "/Game/LivingRoom/Meshes/SM_BLD_Wall_Inner_Room",
    "BLD_Wall_LightBlock": "/Game/LivingRoom/Meshes/SM_BLD_Wall_LightBlock_01a",
    "BLD_Wall_Window_2M": "/Game/LivingRoom/Meshes/SM_BLD_Wall_Window_2M",
    "BLD_Wall_Window_3M": "/Game/LivingRoom/Meshes/SM_BLD_Wall_Window_3M",
    "BLD_Wall_Window_Stairs": "/Game/LivingRoom/Meshes/SM_BLD_Wall_Window_Stairs",
    "BLD_Arch_2M": "/Game/LivingRoom/Meshes/SM_BLD_Arch_2M",
    "BLD_Column_A": "/Game/LivingRoom/Meshes/SM_BLD_Column_01a",
    "BLD_Column_B": "/Game/LivingRoom/Meshes/SM_BLD_Column_01b",
    "BLD_Floor_2M": "/Game/LivingRoom/Meshes/SM_BLD_Floor_2M_01b",
    "BLD_Floor_3M": "/Game/LivingRoom/Meshes/SM_BLD_Floor_3M_01a",
    "BLD_Ceiling_1_5M": "/Game/LivingRoom/Meshes/SM_BLD_Ceiling_1-5M_01d",
    "BLD_Ceiling_3M": "/Game/LivingRoom/Meshes/SM_BLD_Ceiling_3M_02a",
    "BLD_Ceiling_3_5M": "/Game/LivingRoom/Meshes/SM_BLD_Ceiling_3-5M_01c",
    "BLD_Ceiling_4M": "/Game/LivingRoom/Meshes/SM_BLD_Ceiling_4M_01d",
    "BLD_Ceiling_Alt_3M_B": "/Game/LivingRoom/Meshes/SM_BLD_Ceiling_Alt_3M_01b",
    "BLD_Ceiling_Alt_3M_C": "/Game/LivingRoom/Meshes/SM_BLD_Ceiling_Alt_3M_01c",
    "BLD_Stairs_A": "/Game/LivingRoom/Meshes/SM_BLD_Stairs_01a",
    "BLD_Stairs_B": "/Game/LivingRoom/Meshes/SM_BLD_Stairs_01b",
    "BLD_Stairs_C": "/Game/LivingRoom/Meshes/SM_BLD_Stairs_01c",
    "BLD_Stairs_Wall": "/Game/LivingRoom/Meshes/SM_BLD_Stairs_Wall_01b",
    "WindowFrame": "/Game/StarterContent/Props/SM_WindowFrame",
    "GlassWindow": "/Game/StarterContent/Props/SM_GlassWindow",
    "DoorFrame": "/Game/StarterContent/Props/SM_DoorFrame",
    "Door": "/Game/StarterContent/Props/SM_Door",
    "Proto_Door": "/Game/LevelPrototyping/Interactable/Door/Meshes/SM_Door",
    "Proto_Plane": "/Game/LevelPrototyping/Meshes/SM_Plane",
    "Pillar300": "/Game/StarterContent/Props/SM_PillarFrame300",
    "Statue": "/Game/StarterContent/Props/SM_Statue",
    "Bush": "/Game/StarterContent/Props/SM_Bush",
    "Rock": "/Game/StarterContent/Props/SM_Rock",
    "Bookcase_A": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_01a",
    "Bookcase_B": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_01b",
    "Bookcase_C": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_01c",
    "Bookcase_D": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_01d",
    "Bookcase_Cap": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_Cap_01a",
    "Bookcase_Half_A": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_Half_01a",
    "Bookcase_Half_B": "/Game/LivingRoom/Meshes/SM_Wall_Bookcase_Half_01b",
    "Fireplace_Main": "/Game/LivingRoom/Meshes/SM_Wall_Fireplace",
    "Fireplace_Decor_A": "/Game/LivingRoom/Meshes/SM_Fireplace_Deor_01a",
    "Fireplace_Decor_B": "/Game/LivingRoom/Meshes/SM_Fireplace_Deor_01b",
    "Fireplace_Painting": "/Game/LivingRoom/Meshes/SM_Frame_fireplace_01b",
    "Sofa_Chesterfield": "/Game/LivingRoom/Meshes/SM_Sofa_2M_01a",
    "Armchair_Wing_B": "/Game/LivingRoom/Meshes/SM_Sofa_1M_01b",
    "Armchair_Wing_C": "/Game/LivingRoom/Meshes/SM_Sofa_1M_01c",
    "Cushion_A": "/Game/LivingRoom/Meshes/SM_Cushion_01a",
    "Cushion_B": "/Game/LivingRoom/Meshes/SM_Cushion_01b",
    "Cushion_C": "/Game/LivingRoom/Meshes/SM_Cushion_01c",
    "Blanket": "/Game/LivingRoom/Meshes/SM_Blanket_01a",
    "Cloth_A": "/Game/LivingRoom/Meshes/SM_Cloth_01a",
    "Cloth_B": "/Game/LivingRoom/Meshes/SM_Cloth_01b",
    "Carpet_SlateBlue": "/Game/LivingRoom/Meshes/SM_Carpet_01a",
    "Carpet_Runner": "/Game/LivingRoom/Meshes/SM_Carpet_01b",
    "Carpet_Stairs": "/Game/LivingRoom/Meshes/SM_Carpet_Stairs_01a",
    "Curtain_A": "/Game/LivingRoom/Meshes/SM_Curtain_01a",
    "Curtain_B": "/Game/LivingRoom/Meshes/SM_Curtain_01b",
    "Curtain_C": "/Game/LivingRoom/Meshes/SM_Curtain_01c",
    "Table_Side_A": "/Game/LivingRoom/Meshes/SM_Table_01a",
    "Table_Side_B": "/Game/LivingRoom/Meshes/SM_Table_01b",
    "Table_Medium": "/Game/LivingRoom/Meshes/SM_Table_Medium_01a",
    "Cabinet_Tall": "/Game/LivingRoom/Meshes/SM_Cabinet_1M_01a",
    "Shelf_Wall": "/Game/LivingRoom/Meshes/SM_Shelf_01a",
    "Lamp_Tiffany_Green": "/Game/LivingRoom/Meshes/SM_Lamp_Small_01a",
    "Lamp_Tiffany_Purple": "/Game/LivingRoom/Meshes/SM_Lamp_Small_01c",
    "Lamp_Tall_Floor": "/Game/LivingRoom/Meshes/SM_Lamp_Talll_01b",
    "Chandelier_Top": "/Game/LivingRoom/Meshes/SM_Lamp_Top_01d",
    "Sconce_Wall": "/Game/LivingRoom/Meshes/SM_Lamp_Wall_01e",
    "Switch_A": "/Game/LivingRoom/Meshes/SM_Switch_01a",
    "Switch_B": "/Game/LivingRoom/Meshes/SM_Switch_01b",
    "Cable_A": "/Game/LivingRoom/Meshes/SM_Cable_01a",
    "Cable_B": "/Game/LivingRoom/Meshes/SM_Cable_01b",
    "Cable_C": "/Game/LivingRoom/Meshes/SM_Cable_01c",
    "Cable_D": "/Game/LivingRoom/Meshes/SM_Cable_01d",
    "Cable_E": "/Game/LivingRoom/Meshes/SM_Cable_01e",
    "Books_Merged_A": "/Game/LivingRoom/Meshes/SM_Book_Merged_01a",
    "Books_Merged_B": "/Game/LivingRoom/Meshes/SM_Book_Merged_01b",
    "Books_Merged_C": "/Game/LivingRoom/Meshes/SM_Book_Merged_01c",
    "Books_Merged_D": "/Game/LivingRoom/Meshes/SM_Book_Merged_01d",
    "Books_Merged_E": "/Game/LivingRoom/Meshes/SM_Book_Merged_01e",
    "Books_Merged_2A": "/Game/LivingRoom/Meshes/SM_Book_Merged_02a",
    "Books_Merged_Small": "/Game/LivingRoom/Meshes/SM_Book_Merged_Small01",
    "Books_Straight_A": "/Game/LivingRoom/Meshes/SM_Books_Straight_01a",
    "Books_Straight_B": "/Game/LivingRoom/Meshes/SM_Books_Straight_01b",
    "Books_Straight_C": "/Game/LivingRoom/Meshes/SM_Books_Straight_01c",
    "Book_Single_02": "/Game/LivingRoom/Meshes/SM_Book_Single_02",
    "Book_Single_05": "/Game/LivingRoom/Meshes/SM_Book_Single_05",
    "Book_Single_08": "/Game/LivingRoom/Meshes/SM_Book_Single_08",
    "Book_Single_12": "/Game/LivingRoom/Meshes/SM_Book_Single_12",
    "OpenBook": "/Game/LivingRoom/Meshes/SM_OpenBook",
    "Notebook_RedInk_A": "/Game/LivingRoom/Meshes/SM_OpenNotebook_01a",
    "Notebook_RedInk_B": "/Game/LivingRoom/Meshes/SM_OpenNotebook_01b",
    "RedMarker": "/Game/LivingRoom/Meshes/SM_Marker",
    "Keys_Ring": "/Game/LivingRoom/Meshes/SM_Keys_01a",
    "Envelope_Main": "/Game/LivingRoom/Meshes/SM_Envelope",
    "Envelope_A": "/Game/LivingRoom/Meshes/SM_Envelope_01a",
    "Envelope_B": "/Game/LivingRoom/Meshes/SM_Envelope_01b",
    "Envelope_C": "/Game/LivingRoom/Meshes/SM_Envelope_01c",
    "Folder_A": "/Game/LivingRoom/Meshes/SM_Folder_01a",
    "Folder_B": "/Game/LivingRoom/Meshes/SM_Folder_01b",
    "Folder_C": "/Game/LivingRoom/Meshes/SM_Folder_01c",
    "Newspaper": "/Game/LivingRoom/Meshes/SM_Newspaper_01a",
    "Papers_Group": "/Game/LivingRoom/Meshes/SM_Papers_Group",
    "Paper_A": "/Game/LivingRoom/Meshes/SM_Paper_01a",
    "Paper_B": "/Game/LivingRoom/Meshes/SM_Paper_01b",
    "Page_A": "/Game/LivingRoom/Meshes/SM_Page_01a",
    "Page_B": "/Game/LivingRoom/Meshes/SM_Page_01b",
    "Page_C": "/Game/LivingRoom/Meshes/SM_Page_01c",
    "Page_D": "/Game/LivingRoom/Meshes/SM_Page_01d",
    "Page_E": "/Game/LivingRoom/Meshes/SM_Page_01e",
    "Page_F": "/Game/LivingRoom/Meshes/SM_Page_01f",
    "Page_G": "/Game/LivingRoom/Meshes/SM_Page_01g",
    "Page_H": "/Game/LivingRoom/Meshes/SM_Page_01h",
    "Tarot_A": "/Game/LivingRoom/Meshes/SM_Tarot_01a",
    "Tarot_B": "/Game/LivingRoom/Meshes/SM_Tarot_01b",
    "Tarot_C": "/Game/LivingRoom/Meshes/SM_Tarot_01c",
    "Tarot_E": "/Game/LivingRoom/Meshes/SM_Tarot_01e",
    "Tarot_F": "/Game/LivingRoom/Meshes/SM_Tarot_01f",
    "Tarot_G": "/Game/LivingRoom/Meshes/SM_Tarot_01g",
    "Ticket_A": "/Game/LivingRoom/Meshes/SM_Ticket_01a",
    "Ticket_B": "/Game/LivingRoom/Meshes/SM_Ticket_01b",
    "Box_National": "/Game/LivingRoom/Meshes/SM_Box",
    "Box_Cap": "/Game/LivingRoom/Meshes/SM_Box_Cap",
    "Frame_Wall_A": "/Game/LivingRoom/Meshes/SM_Frame_01a",
    "Frame_Wall_D": "/Game/LivingRoom/Meshes/SM_Frame_01d",
    "Frame_Small_C": "/Game/LivingRoom/Meshes/SM_Frame_small_01c",
    "Frame_Small_D": "/Game/LivingRoom/Meshes/SM_Frame_small_01d",
    "Frame_Leaning_A": "/Game/LivingRoom/Meshes/SM_Frame_Merged_01a",
    "Frame_Leaning_B": "/Game/LivingRoom/Meshes/SM_Frame_Merged_01b",
    "Frame_Leaning_C": "/Game/LivingRoom/Meshes/SM_Frame_Merged_01c",
    "Frame_Leaning_D": "/Game/LivingRoom/Meshes/SM_Frame_Merged_01d",
    "Frame_Leaning_E": "/Game/LivingRoom/Meshes/SM_Frame_Merged_01e",
    "Portrait_A": "/Game/LivingRoom/Meshes/SM_Portrait_01a",
    "Portrait_B": "/Game/LivingRoom/Meshes/SM_Portrait_01b",
    "Portrait_C": "/Game/LivingRoom/Meshes/SM_Portrait_01c",
    "Picture_A": "/Game/LivingRoom/Meshes/SM_Picture_01a",
    "Vase_Blue": "/Game/LivingRoom/Meshes/SM_Vase_01a",
    "Vase_Plant_A": "/Game/LivingRoom/Meshes/SM_Vase_Plant_01a",
    "Vase_Plant_B": "/Game/LivingRoom/Meshes/SM_Vase_Plant_01b",
    "Vase_Plant_C": "/Game/LivingRoom/Meshes/SM_Vase_Plant_01c",
    "Vase_Plant_D": "/Game/LivingRoom/Meshes/SM_Vase_Plant_01d",
    "Vase_Plant_E": "/Game/LivingRoom/Meshes/SM_Vase_Plant_01e",
    "Statue_Wood_A": "/Game/LivingRoom/Meshes/SM_Wood_Statue_01a",
    "Statue_Wood_D": "/Game/LivingRoom/Meshes/SM_Wood_Statue_01d",
    "Jar_A": "/Game/LivingRoom/Meshes/SM_Jar_01a",
    "Jar_B": "/Game/LivingRoom/Meshes/SM_Jar_01b",
    "Plate": "/Game/LivingRoom/Meshes/SM_Plate_01a",
    "Cup": "/Game/LivingRoom/Meshes/SM_Cup_01a",
    "Sugar": "/Game/LivingRoom/Meshes/SM_Sugar",
}

ACTOR_SUBSYSTEM = unreal.get_editor_subsystem(unreal.EditorActorSubsystem) if IN_UNREAL_EDITOR else None
EDITOR_ASSET_LIB = unreal.EditorAssetLibrary if IN_UNREAL_EDITOR else None

MESH_CUBE = EDITOR_ASSET_LIB.load_asset("/Engine/BasicShapes/Cube") if IN_UNREAL_EDITOR else None
MESH_CYLINDER = EDITOR_ASSET_LIB.load_asset("/Engine/BasicShapes/Cylinder") if IN_UNREAL_EDITOR else None
MESH_CONE = EDITOR_ASSET_LIB.load_asset("/Engine/BasicShapes/Cone") if IN_UNREAL_EDITOR else None
MESH_SPHERE = EDITOR_ASSET_LIB.load_asset("/Engine/BasicShapes/Sphere") if IN_UNREAL_EDITOR else None

LOADED_MATS = {}
LOADED_MESHES = {}
MESH_BBOX_CACHE = {}


def _try_import_custom_texture(filename, dest_dir="/Game/HorrorMansion/Textures"):
    if not filename or not IN_UNREAL_EDITOR:
        return None
    asset_name = os.path.splitext(filename)[0]
    dest_path = f"{dest_dir}/{asset_name}"
    try:
        if EDITOR_ASSET_LIB.does_asset_exist(dest_path):
            return EDITOR_ASSET_LIB.load_asset(dest_path)
    except Exception:
        pass

    search_dirs = []
    try:
        search_dirs.append(os.path.join(unreal.Paths.project_content_dir(), "HorrorMansion", "SourceTextures"))
    except Exception:
        pass
    try:
        search_dirs.append(os.path.dirname(os.path.abspath(__file__)))
    except Exception:
        pass

    for sdir in search_dirs:
        full_file = os.path.join(sdir, filename)
        if os.path.isfile(full_file):
            try:
                EDITOR_ASSET_LIB.make_directory(dest_dir)
                task = unreal.AssetImportTask()
                task.filename = full_file
                task.destination_path = dest_dir
                task.destination_name = asset_name
                task.replace_existing = True
                task.automated = True
                task.save = True
                unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
                return EDITOR_ASSET_LIB.load_asset(dest_path)
            except Exception:
                pass
    return None


def _create_wired_uv_tiled_material(mat_name, base_key, u_tile, v_tile, roughness_val, custom_tex_file=None):
    if not IN_UNREAL_EDITOR:
        return None
    mat_dir = "/Game/HorrorMansion/Materials"
    full_mat_path = f"{mat_dir}/{mat_name}"
    try:
        if EDITOR_ASSET_LIB.does_asset_exist(full_mat_path):
            return EDITOR_ASSET_LIB.load_asset(full_mat_path)
    except Exception:
        pass

    tex_d = _try_import_custom_texture(custom_tex_file)
    tex_n = None
    if base_key in STARTER_TEX_PATHS:
        d_path, n_path = STARTER_TEX_PATHS[base_key]
        if not tex_d and d_path:
            try:
                tex_d = EDITOR_ASSET_LIB.load_asset(d_path)
            except Exception:
                tex_d = None
        if n_path:
            try:
                tex_n = EDITOR_ASSET_LIB.load_asset(n_path)
            except Exception:
                tex_n = None

    if not tex_d:
        return LOADED_MATS.get(base_key)

    try:
        EDITOR_ASSET_LIB.make_directory(mat_dir)
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        new_mat = asset_tools.create_asset(mat_name, mat_dir, unreal.Material, unreal.MaterialFactoryNew())
        if not new_mat:
            return LOADED_MATS.get(base_key)

        mel = unreal.MaterialEditingLibrary
        tc = mel.create_material_expression(new_mat, unreal.MaterialExpressionTextureCoordinate, -680, 0)
        tc.set_editor_property("u_tiling", float(u_tile))
        tc.set_editor_property("v_tiling", float(v_tile))

        ts_d = mel.create_material_expression(new_mat, unreal.MaterialExpressionTextureSample, -340, -160)
        ts_d.set_editor_property("texture", tex_d)
        if not mel.connect_material_expressions(tc, "", ts_d, "UVs"):
            mel.connect_material_expressions(tc, "", ts_d, "Coordinates")
        mel.connect_material_property(ts_d, "RGB", unreal.MaterialProperty.MP_BASE_COLOR)

        if tex_n:
            ts_n = mel.create_material_expression(new_mat, unreal.MaterialExpressionTextureSample, -340, 140)
            ts_n.set_editor_property("texture", tex_n)
            if not mel.connect_material_expressions(tc, "", ts_n, "UVs"):
                mel.connect_material_expressions(tc, "", ts_n, "Coordinates")
            mel.connect_material_property(ts_n, "RGB", unreal.MaterialProperty.MP_NORMAL)

        rough_const = mel.create_material_expression(new_mat, unreal.MaterialExpressionConstant, -340, 20)
        rough_const.set_editor_property("r", float(roughness_val))
        mel.connect_material_property(rough_const, "", unreal.MaterialProperty.MP_ROUGHNESS)

        mel.recompile_material(new_mat)
        EDITOR_ASSET_LIB.save_loaded_asset(new_mat)
        return new_mat
    except Exception:
        return LOADED_MATS.get(base_key)


def load_starter_and_tiled_materials():
    for key, path in MAT_PATHS.items():
        try:
            LOADED_MATS[key] = EDITOR_ASSET_LIB.load_asset(path)
        except Exception:
            LOADED_MATS[key] = None

    for tiled_key, (base_key, mat_name, u_tiling, v_tiling, rough_val, custom_file) in TILED_MAT_RECIPES.items():
        mat = _create_wired_uv_tiled_material(mat_name, base_key, u_tiling, v_tiling, rough_val, custom_file)
        LOADED_MATS[tiled_key] = mat if mat else LOADED_MATS.get(base_key)

    return LOADED_MATS


def load_leartes_mesh(key):
    path = LEARTES_MESHES.get(key, key)
    if path not in LOADED_MESHES:
        try:
            LOADED_MESHES[path] = EDITOR_ASSET_LIB.load_asset(path)
        except Exception:
            LOADED_MESHES[path] = None
    return LOADED_MESHES[path]


def get_mesh_bbox_metrics(mesh_key):
    path = LEARTES_MESHES.get(mesh_key, mesh_key)
    if path in MESH_BBOX_CACHE:
        return MESH_BBOX_CACHE[path]

    mesh = load_leartes_mesh(mesh_key)
    try:
        bbox = mesh.get_bounding_box()
        mn, mx = bbox.min, bbox.max
        mn_x, mn_y, mn_z = float(mn.x), float(mn.y), float(mn.z)
        mx_x, mx_y, mx_z = float(mx.x), float(mx.y), float(mx.z)
        sx = max(1.0, mx_x - mn_x)
        sy = max(1.0, mx_y - mn_y)
        sz = max(1.0, mx_z - mn_z)
    except Exception:
        mn_x, mn_y, mn_z, mx_x, mx_y, mx_z = -50.0, -50.0, 0.0, 50.0, 50.0, 100.0
        sx, sy, sz = 100.0, 100.0, 100.0

    metrics = (mn_x, mn_y, mn_z, mx_x, mx_y, mx_z, sx, sy, sz)
    MESH_BBOX_CACHE[path] = metrics
    return metrics


def _disable_collision(smc):
    try:
        smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        smc.set_collision_profile_name("NoCollision")
    except Exception:
        pass


def spawn_box(center_xyz, size_xyz, mat, label="Box", folder="HorrorMansion/Architecture", rot_pyr=(0.0, 0.0, 0.0), no_collision=False):
    loc = unreal.Vector(float(center_xyz[0]), float(center_xyz[1]), float(center_xyz[2]))
    rot = unreal.Rotator(pitch=float(rot_pyr[0]), yaw=float(rot_pyr[1]), roll=float(rot_pyr[2]))
    actor = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.StaticMeshActor, loc, rot)
    if not actor:
        return None
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    smc = actor.static_mesh_component
    smc.set_mobility(unreal.ComponentMobility.MOVABLE)
    smc.set_static_mesh(MESH_CUBE)
    if mat:
        try:
            smc.set_material(0, mat)
        except Exception:
            pass
    actor.set_actor_scale3d(unreal.Vector(size_xyz[0] / 100.0, size_xyz[1] / 100.0, size_xyz[2] / 100.0))
    if no_collision:
        _disable_collision(smc)
    smc.set_mobility(unreal.ComponentMobility.STATIC)
    return actor


def spawn_cylinder(center_xyz, radius_cm, height_cm, mat, label="Cylinder", folder="HorrorMansion/Architecture", rot_pyr=(0.0, 0.0, 0.0), no_collision=False):
    loc = unreal.Vector(float(center_xyz[0]), float(center_xyz[1]), float(center_xyz[2]))
    rot = unreal.Rotator(pitch=float(rot_pyr[0]), yaw=float(rot_pyr[1]), roll=float(rot_pyr[2]))
    actor = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.StaticMeshActor, loc, rot)
    if not actor:
        return None
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    smc = actor.static_mesh_component
    smc.set_mobility(unreal.ComponentMobility.MOVABLE)
    smc.set_static_mesh(MESH_CYLINDER)
    if mat:
        try:
            smc.set_material(0, mat)
        except Exception:
            pass
    scale_xy = (radius_cm * 2.0) / 100.0
    actor.set_actor_scale3d(unreal.Vector(scale_xy, scale_xy, height_cm / 100.0))
    if no_collision:
        _disable_collision(smc)
    smc.set_mobility(unreal.ComponentMobility.STATIC)
    return actor


def spawn_cone(center_xyz, radius_cm, height_cm, mat, label="Cone", folder="HorrorMansion/Architecture", rot_pyr=(0.0, 0.0, 0.0), no_collision=False):
    loc = unreal.Vector(float(center_xyz[0]), float(center_xyz[1]), float(center_xyz[2]))
    rot = unreal.Rotator(pitch=float(rot_pyr[0]), yaw=float(rot_pyr[1]), roll=float(rot_pyr[2]))
    actor = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.StaticMeshActor, loc, rot)
    if not actor:
        return None
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    smc = actor.static_mesh_component
    smc.set_mobility(unreal.ComponentMobility.MOVABLE)
    smc.set_static_mesh(MESH_CONE)
    if mat:
        try:
            smc.set_material(0, mat)
        except Exception:
            pass
    scale_xy = (radius_cm * 2.0) / 100.0
    actor.set_actor_scale3d(unreal.Vector(scale_xy, scale_xy, height_cm / 100.0))
    if no_collision:
        _disable_collision(smc)
    smc.set_mobility(unreal.ComponentMobility.STATIC)
    return actor


def spawn_leartes_prop(
    mesh_key,
    target_center_xy,
    target_bottom_z,
    yaw_deg=0.0,
    pitch_deg=0.0,
    roll_deg=0.0,
    scale_xyz=(1.0, 1.0, 1.0),
    align_long_axis_to_yaw=False,
    no_collision=False,
    mat_override=None,
    label="LeartesProp",
    folder="HorrorMansion/LeartesInterior",
):
    mesh = load_leartes_mesh(mesh_key)
    if not mesh:
        return None, target_bottom_z

    mn_x, mn_y, mn_z, mx_x, mx_y, mx_z, sx, sy, _ = get_mesh_bbox_metrics(mesh_key)
    sc_x, sc_y, sc_z = float(scale_xyz[0]), float(scale_xyz[1]), float(scale_xyz[2])
    final_yaw = float(yaw_deg)
    if align_long_axis_to_yaw and (sy * sc_y) > (sx * sc_x) * 1.25:
        final_yaw += 90.0

    local_cx = ((mn_x + mx_x) / 2.0) * sc_x
    local_cy = ((mn_y + mx_y) / 2.0) * sc_y
    local_min_z = mn_z * sc_z
    local_max_z = mx_z * sc_z

    rad = math.radians(final_yaw)
    rot_cx = local_cx * math.cos(rad) - local_cy * math.sin(rad)
    rot_cy = local_cx * math.sin(rad) + local_cy * math.cos(rad)

    wx = float(target_center_xy[0]) - rot_cx
    wy = float(target_center_xy[1]) - rot_cy
    wz = float(target_bottom_z) - local_min_z

    loc = unreal.Vector(wx, wy, wz)
    rot = unreal.Rotator(pitch=float(pitch_deg), yaw=final_yaw, roll=float(roll_deg))
    actor = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.StaticMeshActor, loc, rot)
    if not actor:
        return None, target_bottom_z

    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    smc = actor.static_mesh_component
    smc.set_mobility(unreal.ComponentMobility.MOVABLE)
    smc.set_static_mesh(mesh)
    if mat_override:
        try:
            smc.set_material(0, mat_override)
        except Exception:
            pass
    actor.set_actor_scale3d(unreal.Vector(sc_x, sc_y, sc_z))
    if no_collision:
        _disable_collision(smc)
    smc.set_mobility(unreal.ComponentMobility.STATIC)

    top_z = float(target_bottom_z) + (local_max_z - local_min_z)
    return actor, top_z


def spawn_decal_plane(center_xyz, size_xy, mat, yaw_deg=0.0, label="FloorDecal", folder="HorrorMansion/LeartesInterior"):
    return spawn_box(
        center_xyz=(center_xyz[0], center_xyz[1], center_xyz[2]),
        size_xyz=(size_xy[0], size_xy[1], 1.2),
        mat=mat,
        label=label,
        folder=folder,
        rot_pyr=(0.0, yaw_deg, 0.0),
        no_collision=True,
    )


def spawn_wall_stain_plane(center_xyz, width_cm, height_cm, mat, normal_axis="X", label="WallStain", folder="HorrorMansion/03_Mansion_Exterior"):
    size_xyz = (1.2, width_cm, height_cm) if normal_axis == "X" else (width_cm, 1.2, height_cm)
    return spawn_box(center_xyz, size_xyz, mat, label=label, folder=folder, no_collision=True)


def spawn_point_light(xyz, color_rgb, intensity_lumens, attenuation_radius, label="PointLight", folder="HorrorMansion/08_Lighting", cast_shadows=False):
    loc = unreal.Vector(float(xyz[0]), float(xyz[1]), float(xyz[2]))
    actor = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.PointLight, loc, unreal.Rotator(0, 0, 0))
    if not actor:
        return None
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    lc = actor.point_light_component
    lc.set_mobility(unreal.ComponentMobility.MOVABLE)
    lc.set_editor_property("intensity_units", unreal.LightUnits.LUMENS)
    lc.set_intensity(float(intensity_lumens))
    lc.set_light_color(unreal.LinearColor(color_rgb[0], color_rgb[1], color_rgb[2], 1.0))
    lc.set_attenuation_radius(float(attenuation_radius))
    lc.set_cast_shadows(cast_shadows)
    try:
        lc.set_editor_property("cast_volumetric_shadow", False)
    except Exception:
        pass
    return actor


def spawn_spot_light(xyz, rot_pyr, color_rgb, intensity_lumens, attenuation_radius, inner_angle=22.0, outer_angle=48.0, volumetric_scatter=1.0, label="SpotLight", folder="HorrorMansion/08_Lighting"):
    loc = unreal.Vector(float(xyz[0]), float(xyz[1]), float(xyz[2]))
    rot = unreal.Rotator(pitch=float(rot_pyr[0]), yaw=float(rot_pyr[1]), roll=float(rot_pyr[2]))
    actor = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.SpotLight, loc, rot)
    if not actor:
        return None
    actor.set_actor_label(label)
    actor.set_folder_path(folder)
    lc = actor.spot_light_component
    lc.set_mobility(unreal.ComponentMobility.MOVABLE)
    lc.set_editor_property("intensity_units", unreal.LightUnits.LUMENS)
    lc.set_intensity(float(intensity_lumens))
    lc.set_light_color(unreal.LinearColor(color_rgb[0], color_rgb[1], color_rgb[2], 1.0))
    lc.set_attenuation_radius(float(attenuation_radius))
    lc.set_inner_cone_angle(float(inner_angle))
    lc.set_outer_cone_angle(float(outer_angle))
    lc.set_cast_shadows(True)
    try:
        lc.set_editor_property("cast_volumetric_shadow", True)
        lc.set_editor_property("volumetric_scattering_intensity", float(volumetric_scatter))
    except Exception:
        pass
    return actor


def cleanup_previous_mansion_actors():
    if not ACTOR_SUBSYSTEM:
        return
    env_classes = (
        unreal.DirectionalLight,
        unreal.SkyLight,
        unreal.SkyAtmosphere,
        unreal.ExponentialHeightFog,
        unreal.PostProcessVolume,
        unreal.PlayerStart,
    )
    try:
        all_actors = list(ACTOR_SUBSYSTEM.get_all_level_actors())
        for act in all_actors:
            try:
                f_path = str(act.get_folder_path())
                if f_path.startswith("HorrorMansion") or isinstance(act, env_classes):
                    ACTOR_SUBSYSTEM.destroy_actor(act)
            except Exception:
                pass
    except Exception:
        pass


# ==============================================================================
# 3. 1:1 LEARTES MODULAR WALL, FLOOR & CEILING BUILDERS (ZERO GAP ABOVE DOORS!)
# ==============================================================================

def spawn_leartes_modular_span(
    mesh_key,
    center_xy,
    bottom_z,
    span_len_cm,
    target_height_cm,
    wall_yaw_deg,
    thickness_scale=1.0,
    no_collision=False,
    label="BLD_WallModule",
    folder="HorrorMansion/LeartesModular",
):
    _, _, _, _, _, _, sx, sy, sz = get_mesh_bbox_metrics(mesh_key)
    if sx >= sy:
        scale_xyz = (span_len_cm / sx, thickness_scale, target_height_cm / sz)
        align_yaw = False
    else:
        scale_xyz = (thickness_scale, span_len_cm / sy, target_height_cm / sz)
        align_yaw = True

    return spawn_leartes_prop(
        mesh_key=mesh_key,
        target_center_xy=center_xy,
        target_bottom_z=bottom_z,
        yaw_deg=wall_yaw_deg,
        scale_xyz=scale_xyz,
        align_long_axis_to_yaw=align_yaw,
        no_collision=no_collision,
        label=label,
        folder=folder,
    )


def _subdivide_run_into_leartes_bays(seg_start, seg_end):
    total_len = seg_end - seg_start
    if total_len <= 10.0:
        return []
    n_bays = max(1, int(round(total_len / 300.0)))
    bay_len = total_len / float(n_bays)
    bays = []
    for i in range(n_bays):
        b0 = seg_start + i * bay_len
        b1 = b0 + bay_len
        bays.append((b0, b1, (b0 + b1) / 2.0, bay_len))
    return bays


def build_leartes_modular_wall_along_x(
    y_fixed,
    x_start,
    x_end,
    z_bottom,
    z_top,
    doorways,
    label_prefix,
    folder,
    wall_yaw_deg=0.0,
    fireplace_x=None,
    spawn_pilasters=True,
):
    total_h = z_top - z_bottom
    lower_h = min(340.0, total_h * 0.70)
    upper_h = max(0.0, total_h - lower_h)
    trim_mat = LOADED_MATS.get("wood_walnut")
    wall_mat = LOADED_MATS.get("concrete_grime_tiled")
    sorted_doors = sorted(doorways, key=lambda d: d[0])

    if spawn_pilasters:
        spawn_leartes_modular_span(
            "BLD_Column_A", (x_start + 12.0, y_fixed), z_bottom, 26.0, total_h, wall_yaw_deg,
            thickness_scale=1.0, no_collision=True, label=f"{label_prefix}_ColStart", folder=folder,
        )
        spawn_leartes_modular_span(
            "BLD_Column_A", (x_end - 12.0, y_fixed), z_bottom, 26.0, total_h, wall_yaw_deg,
            thickness_scale=1.0, no_collision=True, label=f"{label_prefix}_ColEnd", folder=folder,
        )

    def _populate_solid_segment(seg_x0, seg_x1, seg_tag):
        seg_len = seg_x1 - seg_x0
        seg_mid = (seg_x0 + seg_x1) / 2.0
        spawn_box((seg_mid, y_fixed, z_bottom + total_h / 2.0), (seg_len, 18.0, total_h), wall_mat, f"{label_prefix}_{seg_tag}_Core", folder)
        spawn_box((seg_mid, y_fixed, z_bottom + 52.0), (seg_len, 24.0, 104.0), trim_mat, f"{label_prefix}_{seg_tag}_Wainscot", folder, no_collision=True)
        spawn_box((seg_mid, y_fixed, z_top - 10.0), (seg_len, 26.0, 20.0), trim_mat, f"{label_prefix}_{seg_tag}_Crown", folder, no_collision=True)

        for b_idx, (b0, _, bcx, blen) in enumerate(_subdivide_run_into_leartes_bays(seg_x0, seg_x1)):
            if fireplace_x is not None and abs(bcx - fireplace_x) < blen * 0.6:
                m_key = "BLD_Wall_Fireplace_2M"
            elif blen >= 335.0:
                m_key = "BLD_Wall_4M"
            elif blen < 170.0:
                m_key = "BLD_Wall_1M"
            else:
                m_key = "BLD_Wall_3M_A" if b_idx % 2 == 0 else "BLD_Wall_3M_B"

            spawn_leartes_modular_span(
                m_key, (bcx, y_fixed), z_bottom, blen, lower_h, wall_yaw_deg,
                thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_{seg_tag}_Bay{b_idx}", folder=folder,
            )
            if upper_h > 10.0:
                spawn_leartes_modular_span(
                    "BLD_Wall_1M", (bcx, y_fixed), z_bottom + lower_h, blen, upper_h, wall_yaw_deg,
                    thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_{seg_tag}_Frieze{b_idx}", folder=folder,
                )
            if spawn_pilasters and b_idx > 0:
                spawn_leartes_modular_span(
                    "BLD_Column_B", (b0, y_fixed), z_bottom, 22.0, total_h, wall_yaw_deg,
                    no_collision=True, label=f"{label_prefix}_{seg_tag}_Pilaster{b_idx}", folder=folder,
                )

    cursor_x = x_start
    for idx, (dcx, _orig_dw, _orig_dh, ajar_yaw, dlabel) in enumerate(sorted_doors):
        dw = 148.0 if ajar_yaw is not None else 190.0
        dh = 228.0 if ajar_yaw is not None else 265.0
        d_left = dcx - dw / 2.0
        d_right = dcx + dw / 2.0
        if d_left > cursor_x + 8.0:
            _populate_solid_segment(cursor_x, d_left, f"Seg{idx}")

        over_h = z_top - (z_bottom + dh)
        if over_h > 8.0:
            spawn_box((dcx, y_fixed, z_bottom + dh + over_h / 2.0), (dw, 18.0, over_h), wall_mat, f"{dlabel}_OverdoorCore", folder)
            spawn_leartes_modular_span(
                "BLD_Wall_1M", (dcx, y_fixed), z_bottom + dh, dw + 12.0, over_h, wall_yaw_deg,
                thickness_scale=1.15, no_collision=True, label=f"{dlabel}_OverdoorPanel", folder=folder,
            )

        spawn_box((d_left + 4.0, y_fixed, z_bottom + dh / 2.0), (12.0, 28.0, dh), trim_mat, f"{dlabel}_JambL", folder, no_collision=True)
        spawn_box((d_right - 4.0, y_fixed, z_bottom + dh / 2.0), (12.0, 28.0, dh), trim_mat, f"{dlabel}_JambR", folder, no_collision=True)
        spawn_box((dcx, y_fixed, z_bottom + dh + 9.0), (dw + 32.0, 34.0, 18.0), trim_mat, f"{dlabel}_Pediment", folder, no_collision=True)
        spawn_leartes_modular_span(
            "BLD_Arch_2M", (dcx, y_fixed), z_bottom, dw + 24.0, dh + 16.0, wall_yaw_deg,
            thickness_scale=1.2, no_collision=True, label=f"{dlabel}_LeartesArch", folder=folder,
        )

        if ajar_yaw is not None:
            spawn_leartes_prop(
                "Door", (d_left + 22.0, y_fixed), z_bottom, yaw_deg=ajar_yaw,
                scale_xyz=(1.08, 1.08, 1.08), no_collision=True, label=f"{dlabel}_DoorMesh", folder=folder,
            )

        cursor_x = d_right

    if x_end > cursor_x + 8.0:
        _populate_solid_segment(cursor_x, x_end, "SegEnd")


def build_leartes_modular_wall_along_y(
    x_fixed,
    y_start,
    y_end,
    z_bottom,
    z_top,
    doorways,
    label_prefix,
    folder,
    wall_yaw_deg=90.0,
    fireplace_y=None,
    spawn_pilasters=True,
):
    total_h = z_top - z_bottom
    lower_h = min(340.0, total_h * 0.70)
    upper_h = max(0.0, total_h - lower_h)
    trim_mat = LOADED_MATS.get("wood_walnut")
    wall_mat = LOADED_MATS.get("concrete_grime_tiled")
    sorted_doors = sorted(doorways, key=lambda d: d[0])

    if spawn_pilasters:
        spawn_leartes_modular_span(
            "BLD_Column_A", (x_fixed, y_start + 12.0), z_bottom, 26.0, total_h, wall_yaw_deg,
            thickness_scale=1.0, no_collision=True, label=f"{label_prefix}_ColStart", folder=folder,
        )
        spawn_leartes_modular_span(
            "BLD_Column_A", (x_fixed, y_end - 12.0), z_bottom, 26.0, total_h, wall_yaw_deg,
            thickness_scale=1.0, no_collision=True, label=f"{label_prefix}_ColEnd", folder=folder,
        )

    def _populate_solid_segment(seg_y0, seg_y1, seg_tag):
        seg_len = seg_y1 - seg_y0
        seg_mid = (seg_y0 + seg_y1) / 2.0
        spawn_box((x_fixed, seg_mid, z_bottom + total_h / 2.0), (18.0, seg_len, total_h), wall_mat, f"{label_prefix}_{seg_tag}_Core", folder)
        spawn_box((x_fixed, seg_mid, z_bottom + 52.0), (24.0, seg_len, 104.0), trim_mat, f"{label_prefix}_{seg_tag}_Wainscot", folder, no_collision=True)
        spawn_box((x_fixed, seg_mid, z_top - 10.0), (26.0, seg_len, 20.0), trim_mat, f"{label_prefix}_{seg_tag}_Crown", folder, no_collision=True)

        for b_idx, (b0, _, bcy, blen) in enumerate(_subdivide_run_into_leartes_bays(seg_y0, seg_y1)):
            if fireplace_y is not None and abs(bcy - fireplace_y) < blen * 0.6:
                m_key = "BLD_Wall_Fireplace_2M"
            elif blen >= 335.0:
                m_key = "BLD_Wall_4M"
            elif blen < 170.0:
                m_key = "BLD_Wall_1M"
            else:
                m_key = "BLD_Wall_3M_A" if b_idx % 2 == 0 else "BLD_Wall_3M_B"

            spawn_leartes_modular_span(
                m_key, (x_fixed, bcy), z_bottom, blen, lower_h, wall_yaw_deg,
                thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_{seg_tag}_Bay{b_idx}", folder=folder,
            )
            if upper_h > 10.0:
                spawn_leartes_modular_span(
                    "BLD_Wall_1M", (x_fixed, bcy), z_bottom + lower_h, blen, upper_h, wall_yaw_deg,
                    thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_{seg_tag}_Frieze{b_idx}", folder=folder,
                )
            if spawn_pilasters and b_idx > 0:
                spawn_leartes_modular_span(
                    "BLD_Column_B", (x_fixed, b0), z_bottom, 22.0, total_h, wall_yaw_deg,
                    no_collision=True, label=f"{label_prefix}_{seg_tag}_Pilaster{b_idx}", folder=folder,
                )

    cursor_y = y_start
    for idx, (dcy, _orig_dw, _orig_dh, ajar_yaw, dlabel) in enumerate(sorted_doors):
        dw = 148.0 if ajar_yaw is not None else 190.0
        dh = 228.0 if ajar_yaw is not None else 265.0
        d_left = dcy - dw / 2.0
        d_right = dcy + dw / 2.0
        if d_left > cursor_y + 8.0:
            _populate_solid_segment(cursor_y, d_left, f"Seg{idx}")

        over_h = z_top - (z_bottom + dh)
        if over_h > 8.0:
            spawn_box((x_fixed, dcy, z_bottom + dh + over_h / 2.0), (18.0, dw, over_h), wall_mat, f"{dlabel}_OverdoorCore", folder)
            spawn_leartes_modular_span(
                "BLD_Wall_1M", (x_fixed, dcy), z_bottom + dh, dw + 12.0, over_h, wall_yaw_deg,
                thickness_scale=1.15, no_collision=True, label=f"{dlabel}_OverdoorPanel", folder=folder,
            )

        spawn_box((x_fixed, d_left + 4.0, z_bottom + dh / 2.0), (28.0, 12.0, dh), trim_mat, f"{dlabel}_JambL", folder, no_collision=True)
        spawn_box((x_fixed, d_right - 4.0, z_bottom + dh / 2.0), (28.0, 12.0, dh), trim_mat, f"{dlabel}_JambR", folder, no_collision=True)
        spawn_box((x_fixed, dcy, z_bottom + dh + 9.0), (34.0, dw + 32.0, 18.0), trim_mat, f"{dlabel}_Pediment", folder, no_collision=True)
        spawn_leartes_modular_span(
            "BLD_Arch_2M", (x_fixed, dcy), z_bottom, dw + 24.0, dh + 16.0, wall_yaw_deg,
            thickness_scale=1.2, no_collision=True, label=f"{dlabel}_LeartesArch", folder=folder,
        )

        if ajar_yaw is not None:
            spawn_leartes_prop(
                "Door", (x_fixed, d_left + 22.0), z_bottom, yaw_deg=ajar_yaw,
                scale_xyz=(1.08, 1.08, 1.08), no_collision=True, label=f"{dlabel}_DoorMesh", folder=folder,
            )

        cursor_y = d_right

    if y_end > cursor_y + 8.0:
        _populate_solid_segment(cursor_y, y_end, "SegEnd")


def spawn_leartes_modular_floor_grid(
    x_min, x_max, y_min, y_max, surface_z, mesh_key="BLD_Floor_3M", label_prefix="BLD_Floor", folder="HorrorMansion/LeartesModular"
):
    _, _, _, _, _, _, sx, sy, sz = get_mesh_bbox_metrics(mesh_key)
    nx = max(1, int(round((x_max - x_min) / 300.0)))
    ny = max(1, int(round((y_max - y_min) / 300.0)))
    tile_x = (x_max - x_min) / float(nx)
    tile_y = (y_max - y_min) / float(ny)

    for ix in range(nx):
        for iy in range(ny):
            cx = x_min + (ix + 0.5) * tile_x
            cy = y_min + (iy + 0.5) * tile_y
            spawn_leartes_prop(
                mesh_key=mesh_key,
                target_center_xy=(cx, cy),
                target_bottom_z=surface_z - sz,
                yaw_deg=0.0,
                scale_xyz=(tile_x / sx, tile_y / sy, 1.0),
                no_collision=True,
                label=f"{label_prefix}_{ix}_{iy}",
                folder=folder,
            )


def spawn_leartes_modular_ceiling_grid(
    x_min, x_max, y_min, y_max, bottom_z, mesh_key="BLD_Ceiling_3M", label_prefix="BLD_Ceiling", folder="HorrorMansion/LeartesModular"
):
    _, _, _, _, _, _, sx, sy, _ = get_mesh_bbox_metrics(mesh_key)
    nx = max(1, int(round((x_max - x_min) / 320.0)))
    ny = max(1, int(round((y_max - y_min) / 320.0)))
    tile_x = (x_max - x_min) / float(nx)
    tile_y = (y_max - y_min) / float(ny)

    for ix in range(nx):
        for iy in range(ny):
            cx = x_min + (ix + 0.5) * tile_x
            cy = y_min + (iy + 0.5) * tile_y
            spawn_leartes_prop(
                mesh_key=mesh_key,
                target_center_xy=(cx, cy),
                target_bottom_z=bottom_z,
                yaw_deg=0.0,
                scale_xyz=(tile_x / sx, tile_y / sy, 1.0),
                no_collision=True,
                label=f"{label_prefix}_{ix}_{iy}",
                folder=folder,
            )


# ==============================================================================
# 4. EXTERIOR WALL, PITCHED ROOF, FULL LEAFY TREE & LAMP HELPERS
# ==============================================================================

def build_wall_along_x(y_fixed, x_start, x_end, z_bottom, z_top, thickness, doorways, wall_mat, trim_mat, label_prefix, folder):
    wall_h = z_top - z_bottom
    wall_mid_z = (z_bottom + z_top) / 2.0
    sorted_doors = sorted(doorways, key=lambda d: d[0])

    cursor_x = x_start
    for idx, (dcx, _dw, _dh, ajar_yaw, dlabel) in enumerate(sorted_doors):
        dw = 148.0 if ajar_yaw is not None else _dw
        dh = 228.0 if ajar_yaw is not None else _dh
        d_left = dcx - dw / 2.0
        d_right = dcx + dw / 2.0
        if d_left > cursor_x + 4.0:
            for b_i, (_, _, bcx, blen) in enumerate(_subdivide_run_into_leartes_bays(cursor_x, d_left)):
                spawn_box((bcx, y_fixed, wall_mid_z), (blen, thickness, wall_h), wall_mat, f"{label_prefix}_Seg_{idx}_{b_i}", folder)
                spawn_box((bcx, y_fixed, z_bottom + 55.0), (blen, thickness + 6.0, 110.0), trim_mat, f"{label_prefix}_Plinth_{idx}_{b_i}", folder)

        lintel_h = z_top - (z_bottom + dh)
        if lintel_h > 4.0:
            lintel_mid_z = z_bottom + dh + lintel_h / 2.0
            spawn_box((dcx, y_fixed, lintel_mid_z), (dw, thickness, lintel_h), wall_mat, f"{label_prefix}_Above_{dlabel}", folder)

        spawn_box((d_left + 4.0, y_fixed, z_bottom + dh / 2.0), (10.0, thickness + 14.0, dh), trim_mat, f"{dlabel}_JambL", folder, no_collision=True)
        spawn_box((d_right - 4.0, y_fixed, z_bottom + dh / 2.0), (10.0, thickness + 14.0, dh), trim_mat, f"{dlabel}_JambR", folder, no_collision=True)
        spawn_box((dcx, y_fixed, z_bottom + dh + 10.0), (dw + 36.0, thickness + 22.0, 22.0), trim_mat, f"{dlabel}_Pediment", folder, no_collision=True)

        if ajar_yaw is not None:
            spawn_leartes_prop("Door", (d_left + 20.0, y_fixed), z_bottom, yaw_deg=ajar_yaw, no_collision=True, label=f"{dlabel}_DoorMesh", folder=folder)

        cursor_x = d_right

    if x_end > cursor_x + 4.0:
        for b_i, (_, _, bcx, blen) in enumerate(_subdivide_run_into_leartes_bays(cursor_x, x_end)):
            spawn_box((bcx, y_fixed, wall_mid_z), (blen, thickness, wall_h), wall_mat, f"{label_prefix}_Seg_End_{b_i}", folder)
            spawn_box((bcx, y_fixed, z_bottom + 55.0), (blen, thickness + 6.0, 110.0), trim_mat, f"{label_prefix}_Plinth_End_{b_i}", folder)


def build_pitched_gable_roof(center_xy, size_xy, eave_z, ridge_rise_z, roof_mat, gable_mat, trim_mat, label_prefix, folder):
    cx, cy = center_xy
    len_x, span_y = size_xy
    half_y = span_y / 2.0
    slope_len = math.hypot(half_y + 45.0, ridge_rise_z)
    roll_deg = math.degrees(math.atan2(ridge_rise_z, half_y + 20.0))

    spawn_box((cx, cy, eave_z + 10.0), (len_x + 50.0, span_y + 50.0, 20.0), trim_mat, f"{label_prefix}_EaveCornice", folder)

    mid_z = eave_z + 14.0 + ridge_rise_z * 0.48
    offset_y = (half_y + 15.0) * 0.52
    spawn_box(
        (cx, cy - offset_y, mid_z), (len_x + 60.0, slope_len, 22.0), roof_mat,
        f"{label_prefix}_SlopeL", folder, rot_pyr=(0.0, 0.0, roll_deg),
    )
    spawn_box(
        (cx, cy + offset_y, mid_z), (len_x + 60.0, slope_len, 22.0), roof_mat,
        f"{label_prefix}_SlopeR", folder, rot_pyr=(0.0, 0.0, -roll_deg),
    )
    spawn_box((cx, cy, eave_z + ridge_rise_z + 18.0), (len_x + 68.0, 28.0, 24.0), trim_mat, f"{label_prefix}_RidgeCap", folder)

    for tier_i in range(4):
        frac = (tier_i + 0.5) / 4.0
        tier_w = max(40.0, span_y * (1.0 - frac * 0.88))
        tier_z = eave_z + 15.0 + frac * (ridge_rise_z - 10.0)
        tier_h = (ridge_rise_z / 4.0) + 12.0
        spawn_box((cx - len_x / 2.0 + 12.0, cy, tier_z), (24.0, tier_w, tier_h), gable_mat, f"{label_prefix}_GableFront_{tier_i}", folder)
        spawn_box((cx + len_x / 2.0 - 12.0, cy, tier_z), (24.0, tier_w, tier_h), gable_mat, f"{label_prefix}_GableRear_{tier_i}", folder)


def build_window_assembly(center_xyz, width_y, height_z, mats, boarded=False, is_front=True, label_prefix="Window"):
    folder = "HorrorMansion/03_Mansion_Exterior/Windows"
    cx, cy, cz = center_xyz
    dir_x = -1.0 if is_front else 1.0
    spawn_box((cx + dir_x * 4.0, cy, cz), (8.0, width_y - 16.0, height_z - 16.0), mats["glass"], f"{label_prefix}_Glass", folder)
    spawn_box((cx + dir_x * 12.0, cy - width_y / 2.0 + 8.0, cz), (28.0, 16.0, height_z), mats["stone_cut_tiled"], f"{label_prefix}_FrameL", folder)
    spawn_box((cx + dir_x * 12.0, cy + width_y / 2.0 - 8.0, cz), (28.0, 16.0, height_z), mats["stone_cut_tiled"], f"{label_prefix}_FrameR", folder)
    spawn_box((cx + dir_x * 16.0, cy, cz - height_z / 2.0 - 8.0), (38.0, width_y + 24.0, 18.0), mats["stone_cut_tiled"], f"{label_prefix}_Sill", folder)
    spawn_box((cx + dir_x * 16.0, cy, cz + height_z / 2.0 + 10.0), (38.0, width_y + 28.0, 22.0), mats["stone_cut_tiled"], f"{label_prefix}_Header", folder)
    spawn_box((cx + dir_x * 18.0, cy - width_y * 0.25, cz + height_z / 2.0 + 28.0), (32.0, width_y * 0.62, 12.0), mats["stone_cut_tiled"], f"{label_prefix}_PedL", folder, rot_pyr=(0.0, 0.0, 18.0))
    spawn_box((cx + dir_x * 18.0, cy + width_y * 0.25, cz + height_z / 2.0 + 28.0), (32.0, width_y * 0.62, 12.0), mats["stone_cut_tiled"], f"{label_prefix}_PedR", folder, rot_pyr=(0.0, 0.0, -18.0))
    spawn_box((cx + dir_x * 8.0, cy, cz), (12.0, 8.0, height_z - 16.0), mats["wood_walnut"], f"{label_prefix}_MullionV", folder)
    spawn_box((cx + dir_x * 8.0, cy, cz + 20.0), (12.0, width_y - 16.0, 8.0), mats["wood_walnut"], f"{label_prefix}_MullionH", folder)
    spawn_wall_stain_plane(
        (cx + dir_x * 16.5, cy, cz - height_z / 2.0 - 52.0), width_y + 10.0, 75.0,
        mats["decal_stain"], normal_axis="X", label=f"{label_prefix}_UnderSillStain", folder=folder,
    )
    if boarded:
        for idx, (dz, roll_deg) in enumerate([(-65.0, 6.0), (-15.0, -4.5), (40.0, 8.5)]):
            spawn_box((cx + dir_x * 22.0, cy, cz + dz), (8.0, width_y + 30.0, 22.0), mats["wood_oak"], f"{label_prefix}_Board_{idx}", folder, rot_pyr=(0.0, 0.0, roll_deg))


def build_classical_column(base_xyz, height_z, radius, mats, label_prefix="Column", folder="HorrorMansion/03_Mansion_Exterior/Portico", mat_override=None):
    bx, by, bz = base_xyz
    c_mat = mat_override if mat_override else mats["stone_cut_tiled"]
    spawn_box((bx, by, bz + 22.0), (radius * 2.6, radius * 2.6, 44.0), c_mat, f"{label_prefix}_Base", folder)
    shaft_h = height_z - 88.0
    spawn_cylinder((bx, by, bz + 44.0 + shaft_h / 2.0), radius, shaft_h, c_mat, f"{label_prefix}_Shaft", folder)
    spawn_box((bx, by, bz + height_z - 22.0), (radius * 2.7, radius * 2.7, 44.0), c_mat, f"{label_prefix}_Capital", folder)


def build_estate_tree_cluster(base_xyz, scale, mats, label_prefix, folder):
    bx, by, bz = base_xyz
    spawn_cylinder((bx, by, bz + 185.0 * scale), 28.0 * scale, 370.0 * scale, mats["wood_walnut"], f"{label_prefix}_Trunk", folder, rot_pyr=(3.5, 18.0, -2.5))
    for b_i, (ox, oy, oz, r, l, rot) in enumerate([
        (0, -52, 310, 14, 210, (28, 40, 0)),
        (42, 55, 330, 12, 195, (-32, -25, 12)),
        (-48, 22, 295, 11, 185, (34, 110, -15)),
    ]):
        spawn_cone((bx + ox * scale, by + oy * scale, bz + oz * scale), r * scale, l * scale, mats["wood_walnut"], f"{label_prefix}_Br_{b_i}", folder, rot_pyr=rot, no_collision=True)

    canopy_offsets = [
        (0.0, 0.0, 395.0, (3.2, 3.2, 2.5), 0.0),
        (-95.0, -75.0, 345.0, (2.7, 2.6, 2.1), 42.0),
        (95.0, 70.0, 355.0, (2.8, 2.7, 2.2), 115.0),
        (-85.0, 80.0, 335.0, (2.6, 2.5, 2.0), 210.0),
        (85.0, -80.0, 340.0, (2.6, 2.5, 2.0), 295.0),
        (0.0, 110.0, 370.0, (2.4, 2.5, 1.9), 75.0),
        (0.0, -110.0, 365.0, (2.4, 2.5, 1.9), 165.0),
    ]
    for c_i, (cx_off, cy_off, cz_off, c_sc, c_yaw) in enumerate(canopy_offsets):
        spawn_leartes_prop(
            "Bush",
            (bx + cx_off * scale, by + cy_off * scale),
            bz + cz_off * scale,
            yaw_deg=c_yaw,
            scale_xyz=(c_sc[0] * scale, c_sc[1] * scale, c_sc[2] * scale),
            no_collision=True,
            label=f"{label_prefix}_CanopyFoliage_{c_i}",
            folder=folder,
        )

    spawn_leartes_prop("Bush", (bx + 55.0, by - 42.0), bz, yaw_deg=25.0, scale_xyz=(1.55 * scale, 1.55 * scale, 1.15 * scale), no_collision=True, label=f"{label_prefix}_BaseBush_A", folder=folder)
    spawn_leartes_prop("Bush", (bx - 48.0, by - 50.0), bz, yaw_deg=140.0, scale_xyz=(1.25 * scale, 1.35 * scale, 0.95 * scale), no_collision=True, label=f"{label_prefix}_BaseBush_B", folder=folder)
    spawn_leartes_prop("Rock", (bx - 42.0, by + 35.0), bz, yaw_deg=60.0, scale_xyz=(0.62 * scale, 0.62 * scale, 0.52 * scale), no_collision=True, label=f"{label_prefix}_BaseRock", folder=folder)


def build_vintage_lamp_post(base_xyz, mats, label_prefix, folder, light_on=True, lean_deg=0.0):
    bx, by, bz = base_xyz
    spawn_box((bx, by, bz + 20.0), (36.0, 36.0, 40.0), mats["stone_cut_tiled"], f"{label_prefix}_Plinth", folder)
    spawn_cylinder((bx, by, bz + 170.0), 7.0, 260.0, mats["metal_rust"], f"{label_prefix}_Pole", folder, rot_pyr=(lean_deg, 0.0, lean_deg * 0.5))
    spawn_box((bx, by, bz + 305.0), (32.0, 32.0, 10.0), mats["metal_rust"], f"{label_prefix}_Crown", folder)
    spawn_box((bx, by, bz + 330.0), (24.0, 24.0, 40.0), mats["glass"], f"{label_prefix}_LanternGlass", folder)
    if light_on:
        spawn_point_light((bx, by, bz + 335.0), (1.0, 0.68, 0.34), 480.0, 1200.0, f"Light_{label_prefix}", cast_shadows=False)


# ==============================================================================
# 5. ATMOSPHERE & OUTER MAFIA-BOSS ESTATE (`02_Outer_MafiaBoss_Estate`)
# ==============================================================================

def build_atmosphere():
    folder = "HorrorMansion/01_Atmosphere_And_Sky"

    night_driver = ACTOR_SUBSYSTEM.spawn_actor_from_class(
        unreal.DirectionalLight,
        unreal.Vector(-2500.0, -1500.0, 3200.0),
        unreal.Rotator(pitch=14.0, yaw=26.0, roll=0.0),
    )
    if night_driver:
        night_driver.set_actor_label("DirectionalLight_NightSunBelowHorizon")
        night_driver.set_folder_path(folder)
        nsc = night_driver.light_component
        nsc.set_mobility(unreal.ComponentMobility.MOVABLE)
        nsc.set_intensity(0.02)
        nsc.set_light_color(unreal.LinearColor(0.15, 0.22, 0.40, 1.0))
        nsc.set_cast_shadows(False)

    spawn_spot_light(
        xyz=(-1800.0, -800.0, 6200.0),
        rot_pyr=(-55.0, 22.0, 0.0),
        color_rgb=(0.42, 0.58, 0.88),
        intensity_lumens=85000.0,
        attenuation_radius=14000.0,
        inner_angle=45.0,
        outer_angle=80.0,
        volumetric_scatter=1.2,
        label="SpotLight_OverheadMoonlight",
        folder=folder,
    )

    sky_atm = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    if sky_atm:
        sky_atm.set_actor_label("SkyAtmosphere_GravesEstate")
        sky_atm.set_folder_path(folder)
        try:
            sac = sky_atm.get_editor_property("component")
            sac.set_editor_property("multi_scattering_factor", 0.05)
            sac.set_editor_property("rayleigh_scattering", unreal.LinearColor(0.015, 0.025, 0.055, 1.0))
        except Exception:
            pass

    sky_light = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1200), unreal.Rotator(0, 0, 0))
    if sky_light:
        sky_light.set_actor_label("SkyLight_AmbientBounce")
        sky_light.set_folder_path(folder)
        slc = sky_light.light_component
        slc.set_mobility(unreal.ComponentMobility.MOVABLE)
        slc.set_intensity(0.28)
        slc.set_light_color(unreal.LinearColor(0.12, 0.18, 0.32, 1.0))
        try:
            slc.set_editor_property("real_time_capture", True)
            slc.set_editor_property("lower_hemisphere_is_solid_color", True)
        except Exception:
            pass

    fog = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 90), unreal.Rotator(0, 0, 0))
    if fog:
        fog.set_actor_label("ExponentialHeightFog_EstateMist")
        fog.set_folder_path(folder)
        fc = fog.component
        fc.set_editor_property("fog_density", 0.025)
        fc.set_editor_property("fog_height_falloff", 0.22)
        try:
            fc.set_editor_property("fog_inscattering_color", unreal.LinearColor(0.012, 0.018, 0.032, 1.0))
        except Exception:
            pass
        try:
            fc.set_editor_property("volumetric_fog", True)
            fc.set_editor_property("volumetric_fog_extinction_scale", 0.90)
        except Exception:
            pass

    ppv = ACTOR_SUBSYSTEM.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 500), unreal.Rotator(0, 0, 0))
    if ppv:
        ppv.set_actor_label("PostProcessVolume_GravesGrading")
        ppv.set_folder_path(folder)
        ppv.set_editor_property("unbound", True)
        settings = ppv.get_editor_property("settings")
        try:
            settings.set_editor_property("override_vignette_intensity", True)
            settings.set_editor_property("vignette_intensity", 0.58)
            settings.set_editor_property("override_auto_exposure_bias", True)
            settings.set_editor_property("auto_exposure_bias", -0.65)
            ppv.set_editor_property("settings", settings)
        except Exception:
            pass

    ps = ACTOR_SUBSYSTEM.spawn_actor_from_class(
        unreal.PlayerStart,
        unreal.Vector(-4480.0, 0.0, 110.0),
        unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0),
    )
    if ps:
        ps.set_actor_label("PlayerStart_CrossGrover_EstateGates")
        ps.set_folder_path(folder)


def build_outer_mafia_boss_estate(mats):
    folder = "HorrorMansion/02_Outer_MafiaBoss_Estate"

    spawn_box((0.0, 0.0, -28.0), (36000.0, 36000.0, 16.0), mats["ground_moss_tiled"], "Ground_OuterHorizonCountrysideSkirt", folder)
    spawn_box((-2450.0, 0.0, -20.0), (7100.0, 13000.0, 40.0), mats["ground_moss_tiled"], "Ground_FrontEstateParkland", folder)
    spawn_box((1200.0, -3690.0, -20.0), (2400.0, 5620.0, 40.0), mats["ground_moss_tiled"], "Ground_WestEstateParkland", folder)
    spawn_box((1200.0, 3690.0, -20.0), (2400.0, 5620.0, 40.0), mats["ground_moss_tiled"], "Ground_EastEstateParkland", folder)
    spawn_box((4200.0, 0.0, -20.0), (3600.0, 13000.0, 40.0), mats["ground_moss_tiled"], "Ground_RearEstateParkland", folder)

    spawn_box((-4650.0, 0.0, 1.5), (520.0, 11200.0, 3.0), mats["stone_cut_tiled"], "Outer_CountryRoad_CurbBed", folder)
    for r_idx, ry in enumerate(range(-5280, 5500, 480)):
        spawn_box((-4650.0, float(ry), 4.0), (480.0, 478.0, 6.0), mats["cobble_road_tiled"], f"Outer_CountryRoad_SqTile_{r_idx}", folder)
        if r_idx % 2 == 0:
            spawn_leartes_prop("Bush", (-4920.0, float(ry) + 60.0), 0.0, yaw_deg=(r_idx * 47) % 360, scale_xyz=(1.8, 1.6, 1.1), no_collision=True, label=f"CountryRoad_VergeWeed_W_{r_idx}", folder=folder)

    spawn_cylinder((-4350.0, -340.0, 95.0), 8.0, 190.0, mats["wood_walnut"], "EstateSignpost_Pole", folder)
    spawn_box((-4350.0, -300.0, 165.0), (12.0, 95.0, 28.0), mats["wood_oak"], "EstateSignpost_GravesCountryHouse", folder)
    spawn_box((-4210.0, -255.0, 55.0), (65.0, 65.0, 110.0), mats["stone_cut_tiled"], "Arrival_GateCourierPlinth", folder)
    spawn_leartes_prop("Folder_A", (-4212.0, -242.0), 110.5, yaw_deg=15.0, no_collision=True, label="Clue_Arrival_CrossInvestigationFolder", folder=folder)
    spawn_leartes_prop("Newspaper", (-4202.0, -268.0), 111.0, yaw_deg=-12.0, no_collision=True, label="Clue_Arrival_NewspaperClipping_GravesDisappearance", folder=folder)

    for side, y_sign in [("Left", -1.0), ("Right", 1.0)]:
        py = y_sign * 260.0
        spawn_box((-4200.0, py, 175.0), (110.0, 110.0, 350.0), mats["brick_old_tiled"], f"MainGate_Pillar_{side}", folder)
        spawn_box((-4200.0, py, 360.0), (130.0, 130.0, 28.0), mats["stone_cut_tiled"], f"MainGate_PillarCap_{side}", folder)
        spawn_box((-4200.0, py, 395.0), (40.0, 40.0, 50.0), mats["glass"], f"MainGate_Lantern_{side}", folder)
        spawn_point_light((-4200.0, py, 415.0), (1.0, 0.65, 0.28), 650.0, 1400.0, f"Light_MainGate_{side}", cast_shadows=False)

    spawn_box((-4140.0, -145.0, 140.0), (14.0, 210.0, 260.0), mats["metal_rust"], "MainIronGate_Left_Ajar", folder, rot_pyr=(0.0, 35.0, 0.0), no_collision=True)
    spawn_box((-4120.0, 135.0, 140.0), (14.0, 210.0, 260.0), mats["metal_rust"], "MainIronGate_Right_Ajar", folder, rot_pyr=(0.0, -48.0, 0.0), no_collision=True)

    for side, y_start, y_end in [("West", -4500.0, -320.0), ("East", 320.0, 4500.0)]:
        for p_y in range(int(y_start), int(y_end), 600):
            seg_end = min(float(y_end), float(p_y + 600))
            seg_len = seg_end - float(p_y)
            seg_mid = (float(p_y) + seg_end) / 2.0
            if seg_len > 20.0:
                spawn_box((-4200.0, seg_mid, 55.0), (60.0, seg_len, 110.0), mats["stone_hewn_tiled"], f"SecurityWall_Base_{side}_{abs(p_y)}", folder)
                spawn_box((-4200.0, seg_mid, 115.0), (68.0, seg_len, 14.0), mats["stone_cut_tiled"], f"SecurityWall_Coping_{side}_{abs(p_y)}", folder)
                spawn_box((-4200.0, seg_mid, 275.0), (12.0, seg_len, 10.0), mats["metal_rust"], f"SecurityFence_TopRail_{side}_{abs(p_y)}", folder)
                for bar_y in range(int(p_y) + 75, int(seg_end) - 40, 85):
                    spawn_cylinder((-4200.0, float(bar_y), 198.0), 4.5, 165.0, mats["metal_rust"], f"SecurityFence_Picket_{side}_{abs(bar_y)}", folder, no_collision=True)
            spawn_box((-4200.0, float(p_y), 165.0), (86.0, 86.0, 330.0), mats["brick_old_tiled"], f"SecurityFence_Pier_{side}_{abs(p_y)}", folder)
            spawn_box((-4200.0, float(p_y), 338.0), (102.0, 102.0, 22.0), mats["stone_cut_tiled"], f"SecurityFence_PierCap_{side}_{abs(p_y)}", folder)

    spawn_box((-3650.0, -850.0, 12.0), (620.0, 620.0, 24.0), mats["stone_hewn_tiled"], "GuardhouseLodge_Floor", folder)
    build_wall_along_x(
        y_fixed=-550.0, x_start=-3950.0, x_end=-3350.0, z_bottom=24.0, z_top=360.0, thickness=24.0,
        doorways=[(-3650.0, 150.0, 230.0, -80.0, "Door_GuardhouseLodge")],
        wall_mat=mats["brick_old_tiled"], trim_mat=mats["stone_cut_tiled"], label_prefix="Guardhouse_Wall_East", folder=folder,
    )
    spawn_box((-3650.0, -1150.0, 192.0), (600.0, 24.0, 336.0), mats["brick_old_tiled"], "Guardhouse_Wall_West", folder)
    spawn_box((-3950.0, -850.0, 192.0), (24.0, 600.0, 336.0), mats["brick_old_tiled"], "Guardhouse_Wall_Front", folder)
    spawn_box((-3350.0, -850.0, 192.0), (24.0, 600.0, 336.0), mats["brick_old_tiled"], "Guardhouse_Wall_Rear", folder)
    build_window_assembly((-3962.0, -850.0, 195.0), 130.0, 165.0, mats, boarded=False, is_front=True, label_prefix="Guardhouse_FrontWin")
    build_pitched_gable_roof(
        center_xy=(-3650.0, -850.0), size_xy=(620.0, 620.0), eave_z=360.0, ridge_rise_z=170.0,
        roof_mat=mats["rock_slate_roof_tiled"], gable_mat=mats["brick_old_tiled"], trim_mat=mats["stone_cut_tiled"],
        label_prefix="Guardhouse_GableRoof", folder=folder,
    )
    spawn_box((-3750.0, -1060.0, 515.0), (85.0, 85.0, 240.0), mats["brick_old_tiled"], "Guardhouse_Chimney", folder)
    _, gh_tbl_z = spawn_leartes_prop("Table_Side_A", (-3650.0, -980.0), 24.0, yaw_deg=0.0, label="Guardhouse_Desk", folder=folder)
    spawn_leartes_prop("Notebook_RedInk_B", (-3650.0, -980.0), gh_tbl_z, yaw_deg=15.0, no_collision=True, label="Clue_Guardhouse_SecurityLog", folder=folder)
    spawn_leartes_prop("Switch_A", (-3620.0, -565.0), 155.0, yaw_deg=180.0, no_collision=True, label="Guardhouse_PowerSwitch", folder=folder)
    spawn_point_light((-3650.0, -850.0, 290.0), (1.0, 0.68, 0.32), 550.0, 900.0, "Light_GuardhouseLodge", cast_shadows=False)

    for d_idx, dx in enumerate(range(-3990, -1950, 400)):
        spawn_box((float(dx), 0.0, 4.0), (402.0, 420.0, 8.0), mats["cobble_drive_tiled"], f"Driveway_LongCobbleAvenue_SqTile_{d_idx}", folder)
        if d_idx % 2 == 1:
            spawn_decal_plane((float(dx), 25.0, 8.8), (180.0, 160.0), mats["decal_grunge"], yaw_deg=d_idx * 35.0, label=f"Driveway_MossPatch_{d_idx}", folder=folder)
    spawn_box((-3025.0, -225.0, 14.0), (2350.0, 24.0, 24.0), mats["stone_cut_tiled"], "Driveway_Curb_L", folder)
    spawn_box((-3025.0, 225.0, 14.0), (2350.0, 24.0, 24.0), mats["stone_cut_tiled"], "Driveway_Curb_R", folder)

    for idx, lx in enumerate([-3800.0, -2950.0, -2100.0]):
        build_vintage_lamp_post((lx, -260.0, 0.0), mats, f"DrivewayLamp_W_{idx}", folder, light_on=True, lean_deg=(1.5 if idx == 1 else 0.0))
        build_vintage_lamp_post((lx, 260.0, 0.0), mats, f"DrivewayLamp_E_{idx}", folder, light_on=(idx != 1), lean_deg=(-2.2 if idx == 1 else 0.0))

    for h_i, h_x in enumerate(range(-3950, -1950, 210)):
        jit_w = ((h_i * 37) % 55) - 25.0
        jit_e = ((h_i * 53) % 55) - 25.0
        sc_w = 1.25 + ((h_i * 19) % 45) * 0.012
        sc_e = 1.20 + ((h_i * 29) % 50) * 0.012
        spawn_leartes_prop("Bush", (float(h_x), -315.0 + jit_w), 0.0, yaw_deg=(h_x * 3) % 360, scale_xyz=(sc_w, sc_w * 1.1, sc_w * 0.85), no_collision=True, label=f"DrivewayHedge_W_{abs(h_x)}", folder=folder)
        spawn_leartes_prop("Bush", (float(h_x) + 35.0, 315.0 + jit_e), 0.0, yaw_deg=(h_x * 5 + 45) % 360, scale_xyz=(sc_e, sc_e * 1.1, sc_e * 0.85), no_collision=True, label=f"DrivewayHedge_E_{abs(h_x)}", folder=folder)
        if h_i % 3 == 0:
            spawn_leartes_prop("Bush", (float(h_x) + 20.0, -215.0), -8.0, yaw_deg=h_i * 60.0, scale_xyz=(0.75, 0.85, 0.35), no_collision=True, label=f"DrivewayCurbWeed_W_{h_i}", folder=folder)
            spawn_leartes_prop("Bush", (float(h_x) - 20.0, 215.0), -8.0, yaw_deg=h_i * 75.0, scale_xyz=(0.75, 0.85, 0.35), no_collision=True, label=f"DrivewayCurbWeed_E_{h_i}", folder=folder)

    spawn_cylinder((-1250.0, 0.0, 4.0), 740.0, 8.0, mats["cobble_forecourt_tiled"], "Forecourt_CarriageCircle", folder)
    spawn_cylinder((-1250.0, 0.0, 28.0), 250.0, 52.0, mats["stone_cut_tiled"], "Fountain_OuterBasin", folder)
    spawn_cylinder((-1250.0, 0.0, 42.0), 220.0, 32.0, mats["water_lake"], "Fountain_ReflectiveWater", folder, no_collision=True)
    spawn_cylinder((-1250.0, 0.0, 110.0), 46.0, 140.0, mats["marble_polished"], "Fountain_CenterPedestal", folder)
    spawn_cylinder((-1250.0, 0.0, 180.0), 120.0, 24.0, mats["marble_polished"], "Fountain_MiddleTierBowl", folder)
    spawn_leartes_prop("Statue", (-1250.0, 0.0), 192.0, yaw_deg=180.0, scale_xyz=(1.2, 1.2, 1.2), label="Fountain_TopClassicalStatue", folder=folder)
    for fw_i, fw_ang in enumerate([25.0, 140.0, 260.0]):
        fw_rad = math.radians(fw_ang)
        spawn_leartes_prop("Bush", (-1250.0 + 255.0 * math.cos(fw_rad), 255.0 * math.sin(fw_rad)), -6.0, yaw_deg=fw_ang, scale_xyz=(0.85, 0.85, 0.45), no_collision=True, label=f"Fountain_NeglectedMossBush_{fw_i}", folder=folder)

    spawn_box((-1400.0, -3100.0, 12.0), (1000.0, 800.0, 24.0), mats["stone_hewn_tiled"], "CoachHouse_Floor", folder)
    build_wall_along_x(
        y_fixed=-2700.0, x_start=-1900.0, x_end=-900.0, z_bottom=24.0, z_top=460.0, thickness=24.0,
        doorways=[(-1400.0, 260.0, 310.0, -85.0, "Door_CoachHouse_Main")],
        wall_mat=mats["brick_old_tiled"], trim_mat=mats["stone_cut_tiled"], label_prefix="CoachHouse_Wall_East", folder=folder,
    )
    spawn_box((-1400.0, -3500.0, 242.0), (1000.0, 24.0, 436.0), mats["brick_old_tiled"], "CoachHouse_Wall_West", folder)
    spawn_box((-1900.0, -3100.0, 242.0), (24.0, 800.0, 436.0), mats["brick_old_tiled"], "CoachHouse_Wall_Front", folder)
    spawn_box((-900.0, -3100.0, 242.0), (24.0, 800.0, 436.0), mats["brick_old_tiled"], "CoachHouse_Wall_Rear", folder)
    build_window_assembly((-1912.0, -3100.0, 245.0), 150.0, 195.0, mats, boarded=True, is_front=True, label_prefix="CoachHouse_FrontWin")
    build_pitched_gable_roof(
        center_xy=(-1400.0, -3100.0), size_xy=(1000.0, 800.0), eave_z=460.0, ridge_rise_z=220.0,
        roof_mat=mats["rock_slate_roof_tiled"], gable_mat=mats["brick_old_tiled"], trim_mat=mats["stone_cut_tiled"],
        label_prefix="CoachHouse_GableRoof", folder=folder,
    )
    spawn_leartes_prop("Box_National", (-1550.0, -3300.0), 24.0, yaw_deg=15.0, scale_xyz=(1.5, 1.5, 1.5), label="CoachHouse_ArchiveBoxes", folder=folder)
    spawn_point_light((-1400.0, -3100.0, 380.0), (1.0, 0.65, 0.28), 750.0, 1300.0, "Light_CoachHouse_Interior", cast_shadows=False)

    spawn_box((-1500.0, 2450.0, 14.0), (540.0, 340.0, 28.0), mats["stone_cut_tiled"], "EstatePool_StoneBorder", folder)
    spawn_box((-1500.0, 2450.0, 22.0), (490.0, 290.0, 16.0), mats["water_lake"], "EstatePool_DarkWater", folder, no_collision=True)
    spawn_box((-1500.0, 3350.0, 190.0), (380.0, 380.0, 360.0), mats["stone_hewn_tiled"], "GravesMausoleum_Crypt", folder)
    spawn_box((-1500.0, 3350.0, 385.0), (420.0, 420.0, 32.0), mats["stone_cut_tiled"], "GravesMausoleum_Cornice", folder)
    build_pitched_gable_roof(
        center_xy=(-1500.0, 3350.0), size_xy=(380.0, 380.0), eave_z=395.0, ridge_rise_z=125.0,
        roof_mat=mats["rock_slate_roof_tiled"], gable_mat=mats["stone_cut_tiled"], trim_mat=mats["stone_cut_tiled"],
        label_prefix="GravesMausoleum_GableRoof", folder=folder,
    )
    for h_i, (hx, hy, h_label) in enumerate([
        (-1350.0, 2880.0, "Headstone_MaraGraves_StagedAccident"),
        (-1500.0, 2880.0, "Headstone_EleanorGraves"),
        (-1650.0, 2880.0, "Headstone_EmptyPlot_MilaGraves"),
        (-1420.0, 3040.0, "Headstone_GravesAncestor_1"),
        (-1580.0, 3040.0, "Headstone_GravesAncestor_2"),
    ]):
        spawn_box((hx, hy, 55.0), (18.0, 65.0, 110.0), mats["marble_polished"], h_label, folder, rot_pyr=(0.0, 0.0, 4.0 if h_i == 2 else -2.0))
        spawn_leartes_prop("Bush", (hx + 20.0, hy + 15.0), -6.0, yaw_deg=h_i * 50.0, scale_xyz=(0.65, 0.65, 0.40), no_collision=True, label=f"{h_label}_GraveWeeds", folder=folder)
    spawn_point_light((-1500.0, 3000.0, 260.0), (0.50, 0.70, 0.98), 680.0, 1200.0, "Light_FamilyGraveyard_MoonFill", cast_shadows=False)

    inner_trees = [
        (-3600, 750, 1.3), (-3100, -1450, 1.4), (-2700, 1100, 1.25), (-2200, -950, 1.35),
        (-1800, 950, 1.2), (-1500, -1350, 1.45), (-950, 1450, 1.3), (-750, -1850, 1.5),
        (-1900, 2100, 1.35), (-1100, 2150, 1.25), (-2100, -2200, 1.4), (-800, 2600, 1.4),
    ]
    outer_horizon_trees = [
        (-3200, -4400, 1.85), (-1600, -4700, 2.05), (400, -4500, 1.90), (2200, -4300, 1.95),
        (-3200, 4400, 1.85), (-1600, 4700, 2.00), (400, 4500, 1.95), (2200, 4300, 1.90),
        (3300, -3200, 2.10), (3600, -1600, 2.25), (3700, 0, 2.30), (3600, 1600, 2.15), (3300, 3200, 2.05),
        (-5400, -3400, 1.90), (-5600, -1800, 1.85), (-5600, 1800, 1.95), (-5400, 3400, 1.85),
    ]
    for idx, (tx, ty, sc) in enumerate(inner_trees + outer_horizon_trees):
        build_estate_tree_cluster((tx, ty, 0.0), sc, mats, f"EstateTree_{idx + 1}", folder)

    for r_i, (rx, ry, r_sc) in enumerate([
        (-2400, -4900, 3.2), (1200, -4800, 3.6), (-2400, 4900, 3.4), (1200, 4800, 3.5),
        (4100, -2200, 4.0), (4200, 800, 4.2), (4000, 2400, 3.8),
    ]):
        spawn_leartes_prop("Rock", (rx, ry), -30.0, yaw_deg=r_i * 55.0, scale_xyz=(r_sc, r_sc * 1.3, r_sc * 0.75), no_collision=True, label=f"HorizonRockOutcrop_{r_i}", folder=folder)


# ==============================================================================
# 6. SUBDIVIDED 3-STORY MANSION EXTERIOR
# ==============================================================================

def _build_exterior_windowed_facade_along_y(
    x_fixed, y_start, y_end, z_bottom, z_top, window_ys, win_w, win_h, win_center_z,
    wall_mat, pilaster_mat, label_prefix, folder, doorway_openings=None
):
    wall_h = z_top - z_bottom
    wall_mid_z = (z_bottom + z_top) / 2.0
    openings = []
    for wy in window_ys:
        openings.append(("window", wy, win_w, z_bottom, win_center_z - win_h / 2.0, win_center_z + win_h / 2.0))
    if doorway_openings:
        for dy, dw, dh in doorway_openings:
            openings.append(("door", dy, dw, z_bottom, z_bottom, z_bottom + dh))
    openings.sort(key=lambda o: o[1])

    cursor_y = y_start
    for idx, (kind, cy, w, _, bot_open_z, top_open_z) in enumerate(openings):
        o_left = cy - w / 2.0
        o_right = cy + w / 2.0
        if o_left > cursor_y + 6.0:
            for b_i, (b0, _, bcy, blen) in enumerate(_subdivide_run_into_leartes_bays(cursor_y, o_left)):
                spawn_box((x_fixed, bcy, wall_mid_z), (30.0, blen, wall_h), wall_mat, f"{label_prefix}_Pier_{idx}_{b_i}", folder)
                if b_i > 0:
                    spawn_box((x_fixed, b0, wall_mid_z), (42.0, 26.0, wall_h), pilaster_mat, f"{label_prefix}_Pilaster_{idx}_{b_i}", folder)

        sill_h = bot_open_z - z_bottom
        if sill_h > 4.0:
            spawn_box((x_fixed, cy, z_bottom + sill_h / 2.0), (30.0, w, sill_h), wall_mat, f"{label_prefix}_SillWall_{idx}", folder)
        head_h = z_top - top_open_z
        if head_h > 4.0:
            spawn_box((x_fixed, cy, top_open_z + head_h / 2.0), (30.0, w, head_h), wall_mat, f"{label_prefix}_HeaderWall_{idx}", folder)

        cursor_y = o_right

    if y_end > cursor_y + 6.0:
        for b_i, (b0, _, bcy, blen) in enumerate(_subdivide_run_into_leartes_bays(cursor_y, y_end)):
            spawn_box((x_fixed, bcy, wall_mid_z), (30.0, blen, wall_h), wall_mat, f"{label_prefix}_PierEnd_{b_i}", folder)
            if b_i > 0:
                spawn_box((x_fixed, b0, wall_mid_z), (42.0, 26.0, wall_h), pilaster_mat, f"{label_prefix}_PilasterEnd_{b_i}", folder)


def _build_subdivided_side_facade_along_x(
    y_fixed, x_start, x_end, z_bottom, z_top, wall_mat, pilaster_mat, label_prefix, folder, window_xs=None, win_w=160.0, win_h=240.0, win_center_z=270.0
):
    wall_h = z_top - z_bottom
    wall_mid_z = (z_bottom + z_top) / 2.0
    openings = sorted(window_xs or [])
    cursor_x = x_start
    for idx, wx in enumerate(openings):
        o_left = wx - win_w / 2.0
        o_right = wx + win_w / 2.0
        if o_left > cursor_x + 6.0:
            for b_i, (b0, _, bcx, blen) in enumerate(_subdivide_run_into_leartes_bays(cursor_x, o_left)):
                spawn_box((bcx, y_fixed, wall_mid_z), (blen, 30.0, wall_h), wall_mat, f"{label_prefix}_Pier_{idx}_{b_i}", folder)
                spawn_box((b0, y_fixed, wall_mid_z), (26.0, 42.0, wall_h), pilaster_mat, f"{label_prefix}_Pilaster_{idx}_{b_i}", folder)
        bot_open_z = win_center_z - win_h / 2.0
        top_open_z = win_center_z + win_h / 2.0
        sill_h = bot_open_z - z_bottom
        if sill_h > 4.0:
            spawn_box((wx, y_fixed, z_bottom + sill_h / 2.0), (win_w, 30.0, sill_h), wall_mat, f"{label_prefix}_Sill_{idx}", folder)
        head_h = z_top - top_open_z
        if head_h > 4.0:
            spawn_box((wx, y_fixed, top_open_z + head_h / 2.0), (win_w, 30.0, head_h), wall_mat, f"{label_prefix}_Header_{idx}", folder)
        spawn_box((wx, y_fixed, win_center_z), (win_w - 12.0, 8.0, win_h - 12.0), LOADED_MATS["glass"], f"{label_prefix}_WinGlass_{idx}", folder)
        cursor_x = o_right

    if x_end > cursor_x + 6.0:
        for b_i, (b0, _, bcx, blen) in enumerate(_subdivide_run_into_leartes_bays(cursor_x, x_end)):
            spawn_box((bcx, y_fixed, wall_mid_z), (blen, 30.0, wall_h), wall_mat, f"{label_prefix}_PierEnd_{b_i}", folder)
            spawn_box((b0, y_fixed, wall_mid_z), (26.0, 42.0, wall_h), pilaster_mat, f"{label_prefix}_PilasterEnd_{b_i}", folder)


def build_mansion_exterior(mats):
    folder = "HorrorMansion/03_Mansion_Exterior"

    for t_i, ty in enumerate([-1650.0, -550.0, 550.0, 1650.0]):
        spawn_box((-280.0, ty, 20.0), (560.0, 1110.0, 40.0), mats["stone_cut_tiled"], f"Terrace_FrontDeck_Bay{t_i}", folder)
    for step_i in range(4):
        sx = -560.0 - step_i * 36.0
        sz = 16.0 - step_i * 4.0
        spawn_box((sx, 0.0, sz / 2.0), (40.0, 780.0 + step_i * 40.0, sz), mats["stone_cut_tiled"], f"Portico_GrandStep_{step_i}", folder)

    for side, y_start, y_end in [("West", -2180, -440), ("East", 440, 2180)]:
        mid_y = (y_start + y_end) / 2.0
        length = abs(y_end - y_start)
        spawn_box((-540.0, mid_y, 95.0), (28.0, length, 14.0), mats["stone_cut_tiled"], f"Terrace_BalustradeRail_{side}", folder)
        for by in range(int(y_start), int(y_end), 120):
            spawn_cylinder((-540.0, float(by), 64.0), 7.0, 50.0, mats["stone_cut_tiled"], f"Terrace_Baluster_{side}_{abs(by)}", folder)

    for idx, cy in enumerate([-520.0, -320.0, -130.0, 130.0, 320.0, 520.0]):
        build_classical_column((-480.0, cy, 40.0), 920.0, 36.0, mats, f"Portico_Col_{idx}", folder)

    spawn_box((-240.0, 0.0, 528.0), (480.0, 1080.0, 24.0), mats["stone_cut_tiled"], "Portico_2F_BalconyFloor", folder)
    spawn_box((-470.0, 0.0, 615.0), (18.0, 1080.0, 14.0), mats["stone_cut_tiled"], "Portico_2F_BalconyRail", folder)
    for by in range(-500, 520, 100):
        spawn_cylinder((-470.0, float(by), 575.0), 6.0, 75.0, mats["stone_cut_tiled"], f"Portico_2F_Baluster_{by + 500}", folder)

    spawn_box((-250.0, 0.0, 1000.0), (520.0, 1180.0, 80.0), mats["stone_cut_tiled"], "Portico_Entablature", folder)
    spawn_box((-495.0, 0.0, 1130.0), (40.0, 920.0, 180.0), mats["brick_old_tiled"], "Portico_PedimentTympanum", folder)
    spawn_box((-500.0, -295.0, 1160.0), (65.0, 660.0, 28.0), mats["stone_cut_tiled"], "Portico_Cornice_L", folder, rot_pyr=(0.0, 0.0, 24.0))
    spawn_box((-500.0, 295.0, 1160.0), (65.0, 660.0, 28.0), mats["stone_cut_tiled"], "Portico_Cornice_R", folder, rot_pyr=(0.0, 0.0, -24.0))
    spawn_box((-250.0, 0.0, 1120.0), (520.0, 1100.0, 160.0), mats["rock_slate_roof_tiled"], "Portico_Roof", folder)
    spawn_leartes_prop("Statue", (-495.0, 0.0), 1245.0, yaw_deg=180.0, scale_xyz=(1.1, 1.1, 1.1), no_collision=True, label="Portico_PedimentAcroterionStatue", folder=folder)

    for side, wy in [("WestWing", -1480.0), ("EastWing", 1480.0)]:
        spawn_cylinder((-245.0, wy, 290.0), 215.0, 500.0, mats["stone_hewn_tiled"], f"FrontTurretBay_Shaft1F_{side}", folder)
        spawn_cylinder((-245.0, wy, 790.0), 212.0, 500.0, mats["brick_old_tiled"], f"FrontTurretBay_Shaft2F_{side}", folder)
        spawn_cylinder((-245.0, wy, 540.0), 226.0, 28.0, mats["stone_cut_tiled"], f"FrontTurretBay_Belt_{side}", folder)
        spawn_cylinder((-245.0, wy, 1045.0), 232.0, 34.0, mats["stone_cut_tiled"], f"FrontTurretBay_Cornice_{side}", folder)
        spawn_cone((-245.0, wy, 1255.0), 265.0, 395.0, mats["rock_slate_roof_tiled"], f"FrontTurretBay_ConicalRoof_{side}", folder)
        build_window_assembly((-455.0, wy, 270.0), 140.0, 235.0, mats, boarded=(side == "WestWing"), is_front=True, label_prefix=f"BayWin_1F_{side}")
        build_window_assembly((-455.0, wy, 780.0), 140.0, 235.0, mats, boarded=False, is_front=True, label_prefix=f"BayWin_2F_{side}")
        spawn_leartes_prop("Bush", (-440.0, wy + 165.0), 40.0, yaw_deg=45.0, scale_xyz=(1.35, 1.2, 1.5), no_collision=True, label=f"TurretBase_OvergrownIvy_{side}", folder=folder)

    for qz in range(80, 1020, 90):
        for qx in [-10.0, 2390.0]:
            for qy in [-2190.0, 2190.0]:
                spawn_box((qx, qy, float(qz)), (55.0, 55.0, 42.0), mats["stone_cut_tiled"], f"CornerQuoin_{int(qx)}_{int(qy)}_{qz}", folder)

    front_win_ys = [-1980.0, -980.0, 980.0, 1980.0]
    _build_exterior_windowed_facade_along_y(
        x_fixed=15.0, y_start=-2200.0, y_end=2200.0, z_bottom=40.0, z_top=540.0,
        window_ys=front_win_ys, win_w=160.0, win_h=250.0, win_center_z=270.0,
        wall_mat=mats["stone_hewn_tiled"], pilaster_mat=mats["stone_cut_tiled"],
        label_prefix="Wall_Front_1F_AshlarStone", folder=folder,
        doorway_openings=[(0.0, 240.0, 320.0)],
    )
    _build_exterior_windowed_facade_along_y(
        x_fixed=15.0, y_start=-2200.0, y_end=2200.0, z_bottom=540.0, z_top=1040.0,
        window_ys=front_win_ys, win_w=160.0, win_h=250.0, win_center_z=780.0,
        wall_mat=mats["brick_old_tiled"], pilaster_mat=mats["stone_cut_tiled"],
        label_prefix="Wall_Front_2F_OldBrick", folder=folder,
        doorway_openings=[(0.0, 220.0, 290.0)],
    )

    for idx, wy in enumerate(front_win_ys):
        build_window_assembly((0.0, wy, 270.0), 160.0, 250.0, mats, boarded=(idx in (0, 2)), is_front=True, label_prefix=f"Win_F1_{idx}")
        build_window_assembly((0.0, wy, 780.0), 160.0, 250.0, mats, boarded=(idx == 3), is_front=True, label_prefix=f"Win_F2_{idx}")

    for b_i, (_, _, bcy, blen) in enumerate(_subdivide_run_into_leartes_bays(-2230.0, 2230.0)):
        spawn_box((-10.0, bcy, 540.0), (50.0, blen, 28.0), mats["stone_cut_tiled"], f"Facade_BeltCourse_Mid_{b_i}", folder)
        spawn_box((-14.0, bcy, 1040.0), (64.0, blen, 38.0), mats["stone_cut_tiled"], f"Facade_Cornice_Roofline_{b_i}", folder)
        if b_i % 2 == 0:
            spawn_wall_stain_plane((-2.0, bcy, 965.0), blen * 0.75, 110.0, mats["decal_grunge"], normal_axis="X", label=f"Facade_CorniceWeathering_{b_i}", folder=folder)

    spawn_leartes_prop("DoorFrame", (15.0, 0.0), 40.0, yaw_deg=90.0, scale_xyz=(1.4, 1.8, 1.4), no_collision=True, label="FrontDoor_GrandFrame", folder=folder)
    spawn_leartes_prop("Door", (25.0, -95.0), 40.0, yaw_deg=18.0, scale_xyz=(1.2, 1.2, 1.35), no_collision=True, label="FrontDoor_OpenL", folder=folder)
    spawn_leartes_prop("Door", (25.0, 95.0), 40.0, yaw_deg=162.0, scale_xyz=(1.2, 1.2, 1.35), no_collision=True, label="FrontDoor_OpenR", folder=folder)

    _build_subdivided_side_facade_along_x(
        -2185.0, 0.0, 2400.0, 40.0, 540.0, mats["stone_hewn_tiled"], mats["stone_cut_tiled"],
        "Wall_Outer_West_1F", folder, window_xs=[320.0, 1680.0], win_center_z=270.0,
    )
    _build_subdivided_side_facade_along_x(
        -2185.0, 0.0, 2400.0, 540.0, 1040.0, mats["brick_old_tiled"], mats["stone_cut_tiled"],
        "Wall_Outer_West_2F", folder, window_xs=[520.0, 1680.0], win_center_z=780.0,
    )
    _build_subdivided_side_facade_along_x(
        2185.0, 0.0, 2400.0, 40.0, 540.0, mats["stone_hewn_tiled"], mats["stone_cut_tiled"],
        "Wall_Outer_East_1F", folder, window_xs=[520.0, 1850.0], win_center_z=270.0,
    )
    _build_subdivided_side_facade_along_x(
        2185.0, 0.0, 2400.0, 540.0, 1040.0, mats["brick_old_tiled"], mats["stone_cut_tiled"],
        "Wall_Outer_East_2F", folder, window_xs=[520.0, 1850.0], win_center_z=780.0,
    )

    _build_exterior_windowed_facade_along_y(
        x_fixed=2385.0, y_start=-2200.0, y_end=2200.0, z_bottom=40.0, z_top=540.0,
        window_ys=[-1150.0, 0.0, 1480.0], win_w=160.0, win_h=250.0, win_center_z=270.0,
        wall_mat=mats["stone_hewn_tiled"], pilaster_mat=mats["stone_cut_tiled"],
        label_prefix="Wall_Outer_Rear_1F", folder=folder,
    )
    _build_exterior_windowed_facade_along_y(
        x_fixed=2385.0, y_start=-2200.0, y_end=2200.0, z_bottom=540.0, z_top=1040.0,
        window_ys=[-1480.0, 0.0, 1480.0], win_w=160.0, win_h=250.0, win_center_z=780.0,
        wall_mat=mats["brick_old_tiled"], pilaster_mat=mats["stone_cut_tiled"],
        label_prefix="Wall_Outer_Rear_2F", folder=folder,
    )
    build_window_assembly((2400.0, -1150.0, 270.0), 160.0, 250.0, mats, boarded=False, is_front=False, label_prefix="Win_Rear_CorridorEnd")

    spawn_box((1200.0, 0.0, 1054.0), (2440.0, 4440.0, 24.0), mats["rock_slate_roof_tiled"], "Roof_Mansard_FullDarkBasaltDeck", folder)
    for r_i, ry in enumerate([-1620.0, -540.0, 540.0, 1620.0]):
        spawn_box((115.0, ry, 1215.0), (380.0, 1085.0, 28.0), mats["rock_slate_roof_tiled"], f"Roof_Mansard_FrontSlope_Bay{r_i}", folder, rot_pyr=(58.0, 0.0, 0.0))
        spawn_box((2285.0, ry, 1215.0), (380.0, 1085.0, 28.0), mats["rock_slate_roof_tiled"], f"Roof_Mansard_RearSlope_Bay{r_i}", folder, rot_pyr=(-58.0, 0.0, 0.0))
        spawn_box((1200.0, ry, 1210.0), (2180.0, 1080.0, 290.0), mats["rock_slate_roof_tiled"], f"Roof_Mansard_LowerCore_Bay{r_i}", folder)
        spawn_box((1200.0, ry, 1410.0), (2060.0, 1040.0, 120.0), mats["rock_slate_roof_tiled"], f"Roof_Mansard_Upper_Bay{r_i}", folder)
        spawn_box((1200.0, ry - 540.0, 1245.0), (2260.0, 22.0, 360.0), mats["stone_cut_tiled"], f"Roof_Mansard_Rib_{r_i}", folder)
    spawn_box((1200.0, -2105.0, 1215.0), (2180.0, 380.0, 28.0), mats["rock_slate_roof_tiled"], "Roof_Mansard_WestSlope", folder, rot_pyr=(0.0, 0.0, -58.0))
    spawn_box((1200.0, 2105.0, 1215.0), (2180.0, 380.0, 28.0), mats["rock_slate_roof_tiled"], "Roof_Mansard_EastSlope", folder, rot_pyr=(0.0, 0.0, 58.0))
    spawn_box((1200.0, 0.0, 1362.0), (2240.0, 4240.0, 26.0), mats["stone_cut_tiled"], "Roof_Mansard_MidCorniceBand", folder)
    spawn_box((1200.0, 0.0, 1485.0), (1960.0, 3960.0, 32.0), mats["metal_rust"], "Roof_IronCresting", folder)

    for d_idx, dy in enumerate([-1850.0, -1150.0, -550.0, 550.0, 1150.0, 1850.0]):
        spawn_box((95.0, dy, 1215.0), (190.0, 150.0, 190.0), mats["brick_old_tiled"], f"RoofDormer_{d_idx}", folder)
        spawn_box((2.0, dy, 1215.0), (16.0, 110.0, 130.0), mats["glass"], f"RoofDormerGlass_{d_idx}", folder)
        spawn_box((95.0, dy, 1320.0), (210.0, 175.0, 28.0), mats["stone_cut_tiled"], f"RoofDormerCap_{d_idx}", folder)

    spawn_box((450.0, 0.0, 1680.0), (540.0, 540.0, 460.0), mats["brick_old_tiled"], "Tower_CentralShaft", folder)
    spawn_box((450.0, 0.0, 1920.0), (600.0, 600.0, 36.0), mats["stone_cut_tiled"], "Tower_Cornice", folder)
    spawn_cone((450.0, 0.0, 2220.0), 390.0, 540.0, mats["rock_slate_roof_tiled"], "Tower_DarkBasaltSpire", folder)
    spawn_cylinder((175.0, 0.0, 1720.0), 95.0, 12.0, mats["marble_polished"], "Tower_ClockFace_Front", folder, rot_pyr=(90.0, 0.0, 0.0))
    spawn_point_light((120.0, 0.0, 1720.0), (1.0, 0.85, 0.55), 1400.0, 950.0, "Light_ClockTower_Glow", folder)

    for side, cy in [("West", -1850.0), ("East", 1850.0)]:
        spawn_box((600.0, cy, 1640.0), (140.0, 140.0, 420.0), mats["brick_old_tiled"], f"Chimney_{side}_F", folder)
        spawn_box((600.0, cy, 1860.0), (165.0, 165.0, 24.0), mats["stone_cut_tiled"], f"ChimneyCap_{side}_F", folder)
        spawn_box((1850.0, cy, 1640.0), (140.0, 140.0, 420.0), mats["brick_old_tiled"], f"Chimney_{side}_R", folder)
        spawn_box((1850.0, cy, 1860.0), (165.0, 165.0, 24.0), mats["stone_cut_tiled"], f"ChimneyCap_{side}_R", folder)

    spawn_box((900.0, 2475.0, 45.0), (1000.0, 550.0, 90.0), mats["stone_hewn_tiled"], "Conservatory_StoneBase", folder)
    spawn_box((900.0, 2740.0, 240.0), (980.0, 12.0, 300.0), mats["glass"], "Conservatory_GlassWall", folder)
    spawn_box((900.0, 2475.0, 410.0), (1000.0, 550.0, 40.0), mats["glass"], "Conservatory_GlassRoof", folder)
    for rib_x in range(450, 1400, 180):
        spawn_box((float(rib_x), 2475.0, 240.0), (14.0, 560.0, 360.0), mats["metal_rust"], f"Conservatory_IronRib_{rib_x}", folder)
        spawn_leartes_prop("Vase_Plant_B", (float(rib_x), 2550.0), 90.0, yaw_deg=rib_x % 180, scale_xyz=(1.4, 1.4, 1.4), label=f"Conservatory_Plant_{rib_x}", folder=folder)


# ==============================================================================
# 7. GRAND FOYER (`04_Grand_Foyer_EntranceHall`)
# ==============================================================================

def build_grand_foyer(mats):
    folder = "HorrorMansion/04_Grand_Foyer_EntranceHall"

    spawn_box((910.0, 0.0, 20.0), (1820.0, 4360.0, 40.0), mats["wood_floor_polished_tiled"], "Floor_1F_MainSlab", folder)
    spawn_box((2100.0, -1515.0, 20.0), (560.0, 1330.0, 40.0), mats["wood_floor_polished_tiled"], "Floor_1F_RearWestSlab", folder)
    spawn_box((2100.0, 915.0, 20.0), (560.0, 2530.0, 40.0), mats["wood_floor_polished_tiled"], "Floor_1F_RearEastSlab", folder)

    for ix in range(11):
        for iy in range(-5, 6):
            tx = 120.0 + ix * 152.0
            ty = iy * 152.0
            tile_mat = mats["marble_polished"] if (ix + iy) % 2 == 0 else mats["rock_basalt"]
            spawn_box((tx, ty, 41.5), (150.0, 150.0, 3.0), tile_mat, f"CheckerTile_{ix}_{iy + 5}", folder, no_collision=True)

    spawn_box((1200.0, 0.0, 1038.0), (2360.0, 4360.0, 12.0), mats["wood_walnut"], "Ceiling_Main_RoofSeal", folder)
    spawn_leartes_modular_ceiling_grid(
        40.0, 1800.0, -880.0, 880.0, 1022.0, mesh_key="BLD_Ceiling_4M", label_prefix="Foyer_LeartesCeiling", folder=folder
    )

    build_leartes_modular_wall_along_y(
        x_fixed=36.0, y_start=-880.0, y_end=880.0, z_bottom=40.0, z_top=540.0,
        doorways=[(0.0, 220.0, 290.0, None, "Foyer_FrontEntranceArch")],
        label_prefix="FoyerWall_1F_FrontInner", folder=folder, wall_yaw_deg=-90.0,
    )
    build_leartes_modular_wall_along_y(
        x_fixed=36.0, y_start=-880.0, y_end=880.0, z_bottom=540.0, z_top=1030.0,
        doorways=[(0.0, 190.0, 265.0, None, "Foyer_2F_BalconyArch")],
        label_prefix="FoyerWall_2F_FrontInner", folder=folder, wall_yaw_deg=-90.0,
    )

    build_leartes_modular_wall_along_x(
        y_fixed=-880.0, x_start=36.0, x_end=1800.0, z_bottom=40.0, z_top=540.0,
        doorways=[
            (480.0, 148.0, 228.0, -85.0, "Door_1F_DiningRoom"),
            (1480.0, 190.0, 265.0, None, "Arch_1F_ToFootprintCorridor"),
        ],
        label_prefix="FoyerWall_1F_West", folder=folder, wall_yaw_deg=0.0,
    )
    build_leartes_modular_wall_along_x(
        y_fixed=-880.0, x_start=36.0, x_end=1800.0, z_bottom=540.0, z_top=1030.0,
        doorways=[
            (480.0, 148.0, 228.0, -85.0, "Door_2F_MasterBedroom"),
            (1480.0, 148.0, 228.0, -85.0, "Door_2F_AtticHobbyRoom"),
        ],
        label_prefix="FoyerWall_2F_West", folder=folder, wall_yaw_deg=0.0,
    )
    build_leartes_modular_wall_along_x(
        y_fixed=880.0, x_start=36.0, x_end=1800.0, z_bottom=40.0, z_top=540.0,
        doorways=[
            (480.0, 148.0, 228.0, 85.0, "Door_1F_PrivateStudy"),
            (1480.0, 148.0, 228.0, 85.0, "Door_1F_SecretBackRoom"),
        ],
        label_prefix="FoyerWall_1F_East", folder=folder, wall_yaw_deg=180.0,
    )
    build_leartes_modular_wall_along_x(
        y_fixed=880.0, x_start=36.0, x_end=1800.0, z_bottom=540.0, z_top=1030.0,
        doorways=[
            (480.0, 148.0, 228.0, 85.0, "Door_2F_MilaBedroom_Unlabeled"),
            (1480.0, 148.0, 228.0, 85.0, "Door_2F_MaraBedroom"),
        ],
        label_prefix="FoyerWall_2F_East", folder=folder, wall_yaw_deg=180.0,
    )
    build_leartes_modular_wall_along_y(
        x_fixed=1800.0, y_start=-880.0, y_end=880.0, z_bottom=40.0, z_top=540.0,
        doorways=[(-500.0, 148.0, 228.0, -85.0, "Door_HiddenBasementPassage")],
        label_prefix="FoyerWall_1F_Rear", folder=folder, wall_yaw_deg=90.0,
    )
    build_leartes_modular_wall_along_y(
        x_fixed=1800.0, y_start=-880.0, y_end=880.0, z_bottom=540.0, z_top=1030.0,
        doorways=[],
        label_prefix="FoyerWall_2F_Rear", folder=folder, wall_yaw_deg=90.0,
    )

    for s in range(15):
        sx = 815.0 + s * 34.0
        sz = 44.0 + (s + 1) * 16.0
        spawn_box((sx, 0.0, (sz + 40.0) / 2.0), (36.0, 316.0, sz - 40.0), mats["marble_polished"], f"Stair_Lower_Riser_{s}", folder)
        spawn_box((sx - 2.0, 0.0, sz - 2.0), (40.0, 326.0, 5.0), mats["wood_walnut_polished"], f"Stair_Lower_Tread_{s}", folder)
        spawn_box((sx - 2.5, 0.0, sz + 1.0), (41.0, 140.0, 2.0), mats["leartes_tile4"], f"Stair_Lower_Runner_{s}", folder, no_collision=True)

    for side, sy in [("L", -168.0), ("R", 168.0)]:
        spawn_box((795.0, sy, 105.0), (28.0, 28.0, 125.0), mats["wood_walnut"], f"Stair_NewelBot_{side}", folder)
        spawn_box((795.0, sy, 174.0), (20.0, 20.0, 20.0), mats["metal_gold"], f"Stair_FinialBot_{side}", folder)
        spawn_box((1055.0, sy, 165.0), (545.0, 18.0, 42.0), mats["wood_walnut"], f"Stair_LowerStringer_{side}", folder, rot_pyr=(25.2, 0.0, 0.0))
        spawn_box((1060.0, sy, 238.0), (555.0, 14.0, 14.0), mats["wood_walnut"], f"Stair_SlopedRail_{side}", folder, rot_pyr=(25.2, 0.0, 0.0))
        for b_i in range(12):
            bx = 830.0 + b_i * 40.0
            bz = 95.0 + b_i * 18.5
            spawn_cylinder((bx, sy, bz), 4.0, 72.0, mats["marble_polished"], f"Stair_LowerBaluster_{side}_{b_i}", folder, no_collision=True)

    for idx, sy in enumerate([-210.0, 210.0]):
        spawn_leartes_modular_span(
            "BLD_Column_A", (1310.0, sy), 40.0, 42.0, 490.0, 0.0,
            label=f"Foyer_LeartesStairCol_{idx}", folder=folder,
        )

    spawn_box((1545.0, 0.0, 274.0), (490.0, 640.0, 20.0), mats["wood_walnut_polished"], "Stair_CentralLanding", folder)
    spawn_leartes_modular_floor_grid(1300.0, 1785.0, -320.0, 320.0, 286.0, mesh_key="BLD_Floor_2M", label_prefix="Landing_LeartesFloor", folder=folder)
    spawn_leartes_prop("Carpet_SlateBlue", (1545.0, 0.0), 286.5, yaw_deg=0.0, scale_xyz=(1.1, 1.35, 1.0), no_collision=True, label="Landing_CenterRug", folder=folder)

    for s in range(15):
        sy_off = 310.0 + s * 23.0
        sz = 284.0 + (s + 1) * 16.4
        spawn_box((1580.0, -sy_off, sz - 8.0), (260.0, 28.0, 18.0), mats["wood_walnut_polished"], f"Stair_UpperW_Step_{s}", folder)
        spawn_box((1580.0, -sy_off, sz + 1.5), (120.0, 29.0, 2.0), mats["leartes_tile4"], f"Stair_UpperW_Runner_{s}", folder, no_collision=True)
        spawn_box((1580.0, sy_off, sz - 8.0), (260.0, 28.0, 18.0), mats["wood_walnut_polished"], f"Stair_UpperE_Step_{s}", folder)
        spawn_box((1580.0, sy_off, sz + 1.5), (120.0, 29.0, 2.0), mats["leartes_tile4"], f"Stair_UpperE_Runner_{s}", folder, no_collision=True)

    spawn_box((1455.0, -470.0, 465.0), (12.0, 390.0, 12.0), mats["wood_walnut"], "Stair_UpperW_Rail", folder, rot_pyr=(0.0, 0.0, -35.5))
    spawn_box((1455.0, 470.0, 465.0), (12.0, 390.0, 12.0), mats["wood_walnut"], "Stair_UpperE_Rail", folder, rot_pyr=(0.0, 0.0, 35.5))

    spawn_box((910.0, -755.0, 528.0), (1760.0, 250.0, 24.0), mats["wood_floor_polished_tiled"], "Mezzanine_Floor_West", folder)
    spawn_box((910.0, 755.0, 528.0), (1760.0, 250.0, 24.0), mats["wood_floor_polished_tiled"], "Mezzanine_Floor_East", folder)
    spawn_box((1320.0, 0.0, 528.0), (200.0, 1260.0, 24.0), mats["wood_floor_polished_tiled"], "Mezzanine_OverheadBridge", folder)
    spawn_box((130.0, 0.0, 528.0), (200.0, 1260.0, 24.0), mats["wood_floor_polished_tiled"], "Mezzanine_FrontBridge", folder)
    spawn_leartes_modular_floor_grid(40.0, 1780.0, -875.0, -630.0, 542.0, mesh_key="BLD_Floor_2M", label_prefix="MezzW_LeartesFloor", folder=folder)
    spawn_leartes_modular_floor_grid(40.0, 1780.0, 630.0, 875.0, 542.0, mesh_key="BLD_Floor_2M", label_prefix="MezzE_LeartesFloor", folder=folder)

    spawn_box((725.0, -635.0, 625.0), (990.0, 12.0, 12.0), mats["wood_walnut"], "Mezzanine_Rail_WestInner", folder)
    spawn_box((725.0, 635.0, 625.0), (990.0, 12.0, 12.0), mats["wood_walnut"], "Mezzanine_Rail_EastInner", folder)
    spawn_box((1225.0, 0.0, 625.0), (12.0, 1260.0, 12.0), mats["wood_walnut"], "Mezzanine_Rail_BridgeFront", folder)
    spawn_box((1415.0, 0.0, 625.0), (12.0, 1260.0, 12.0), mats["wood_walnut"], "Mezzanine_Rail_BridgeBack", folder)
    for by in range(-580, 600, 80):
        spawn_cylinder((1225.0, by, 580.0), 4.0, 88.0, mats["wood_walnut"], f"BridgeBaluster_F_{by + 600}", folder)
        spawn_cylinder((1415.0, by, 580.0), 4.0, 88.0, mats["wood_walnut"], f"BridgeBaluster_B_{by + 600}", folder)

    spawn_leartes_prop("Chandelier_Top", (680.0, 0.0), 820.0, scale_xyz=(1.8, 1.8, 1.8), no_collision=True, label="Foyer_LeartesChandelier", folder=folder)
    spawn_leartes_prop("Carpet_Runner", (460.0, 0.0), 43.0, yaw_deg=0.0, scale_xyz=(1.4, 1.4, 1.0), no_collision=True, label="Foyer_LeartesRunnerRug", folder=folder)
    spawn_leartes_prop("Sconce_Wall", (1778.0, 0.0), 420.0, yaw_deg=-90.0, scale_xyz=(1.3, 1.3, 1.3), no_collision=True, label="Foyer_LandingRedSconce", folder=folder)
    spawn_leartes_prop("Frame_Wall_D", (1778.0, -175.0), 380.0, yaw_deg=-90.0, scale_xyz=(1.3, 1.3, 1.3), no_collision=True, label="Foyer_LandingPainting_L", folder=folder)
    spawn_leartes_prop("Frame_Wall_A", (1778.0, 175.0), 380.0, yaw_deg=-90.0, scale_xyz=(1.3, 1.3, 1.3), no_collision=True, label="Foyer_LandingPainting_R", folder=folder)

    spawn_leartes_prop("Switch_A", (55.0, 220.0), 155.0, yaw_deg=-90.0, no_collision=True, label="Foyer_EntranceLightSwitch", folder=folder)
    spawn_leartes_prop("Cable_A", (740.0, 420.0), 43.0, yaw_deg=45.0, no_collision=True, label="Foyer_ActiveLampPowerCable", folder=folder)

    _, clk_top = spawn_leartes_prop("Cabinet_Tall", (880.0, -810.0), 43.0, yaw_deg=90.0, scale_xyz=(1.05, 1.05, 1.28), label="Foyer_StoppedGrandfatherClock", folder=folder)
    spawn_leartes_prop("Statue_Wood_A", (880.0, -810.0), clk_top, yaw_deg=90.0, no_collision=True, label="Foyer_ClockCrest", folder=folder)
    spawn_leartes_prop("Page_A", (925.0, -845.0), 145.0, pitch_deg=90.0, yaw_deg=90.0, no_collision=True, label="Clue_HiddenDocument_BehindClock_StoppedClockFact", folder=folder)
    spawn_leartes_prop("Picture_A", (1650.0, -858.0), 175.0, yaw_deg=90.0, scale_xyz=(1.25, 1.25, 1.25), no_collision=True, label="Clue_Photo_RevealingHiddenBasementPassage", folder=folder)

    _, ent_tbl_z = spawn_leartes_prop("Table_Medium", (760.0, 380.0), 43.0, yaw_deg=90.0, align_long_axis_to_yaw=True, label="Foyer_EntranceConsoleTable", folder=folder)
    spawn_leartes_prop("Lamp_Tiffany_Green", (760.0, 435.0), ent_tbl_z, yaw_deg=0.0, no_collision=True, label="Foyer_SmallStairLamp_SwitchedOn", folder=folder)
    spawn_leartes_prop("Newspaper", (755.0, 380.0), ent_tbl_z, yaw_deg=14.0, no_collision=True, label="Foyer_EntranceNewspaper", folder=folder)
    spawn_leartes_prop("Frame_Small_C", (762.0, 325.0), ent_tbl_z, yaw_deg=-20.0, no_collision=True, label="Foyer_OldFamilyPhotograph_Table", folder=folder)

    spawn_leartes_prop("Portrait_A", (260.0, -858.0), 195.0, yaw_deg=90.0, scale_xyz=(1.45, 1.45, 1.45), no_collision=True, label="Clue_Portrait_VictorGraves_FounderOfGravesHoldings", folder=folder)
    spawn_leartes_prop("Portrait_B", (260.0, 858.0), 195.0, yaw_deg=-90.0, scale_xyz=(1.4, 1.4, 1.4), no_collision=True, label="Foyer_Portrait_MaraGraves", folder=folder)
    spawn_leartes_prop("Frame_Wall_A", (1020.0, 858.0), 205.0, yaw_deg=-90.0, scale_xyz=(1.3, 1.3, 1.3), no_collision=True, label="Foyer_FamilyPhoto_EastWall", folder=folder)


# ==============================================================================
# 8. GROUND FLOOR ROOMS (`05_GroundFloor_LeartesRooms`)
# ==============================================================================

def _line_exterior_room_wall_along_x(y_fixed, x_start, x_end, z_bottom, z_top, window_xs, wall_yaw_deg, fireplace_x, label_prefix, folder):
    total_h = z_top - z_bottom
    lower_h = min(340.0, total_h * 0.70)
    upper_h = max(0.0, total_h - lower_h)
    trim_mat = LOADED_MATS.get("wood_walnut")
    wall_mat = LOADED_MATS.get("concrete_grime_tiled")

    seg_len = x_end - x_start
    seg_mid = (x_start + x_end) / 2.0
    spawn_box((seg_mid, y_fixed, z_bottom + 50.0), (seg_len, 16.0, 100.0), trim_mat, f"{label_prefix}_Wainscot", folder, no_collision=True)
    spawn_box((seg_mid, y_fixed, z_top - 10.0), (seg_len, 22.0, 20.0), trim_mat, f"{label_prefix}_Crown", folder, no_collision=True)

    for b_idx, (b0, _, bcx, blen) in enumerate(_subdivide_run_into_leartes_bays(x_start, x_end)):
        has_win = any(abs(bcx - wx) < blen * 0.55 for wx in (window_xs or []))
        if fireplace_x is not None and abs(bcx - fireplace_x) < blen * 0.55:
            m_key = "BLD_Wall_Fireplace_2M"
        elif has_win:
            m_key = "BLD_Wall_Window_3M"
        else:
            m_key = "BLD_Wall_3M_A" if b_idx % 2 == 0 else "BLD_Wall_3M_B"

        if not has_win:
            spawn_box((bcx, y_fixed, z_bottom + total_h / 2.0), (blen, 12.0, total_h), wall_mat, f"{label_prefix}_Backer_{b_idx}", folder, no_collision=True)

        spawn_leartes_modular_span(
            m_key, (bcx, y_fixed), z_bottom, blen, lower_h, wall_yaw_deg,
            thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_Bay{b_idx}", folder=folder,
        )
        if upper_h > 10.0:
            spawn_leartes_modular_span(
                "BLD_Wall_1M", (bcx, y_fixed), z_bottom + lower_h, blen, upper_h, wall_yaw_deg,
                thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_Frieze{b_idx}", folder=folder,
            )
        if b_idx > 0:
            spawn_leartes_modular_span(
                "BLD_Column_B", (b0, y_fixed), z_bottom, 22.0, total_h, wall_yaw_deg,
                no_collision=True, label=f"{label_prefix}_Pilaster{b_idx}", folder=folder,
            )


def _line_exterior_room_wall_along_y(x_fixed, y_start, y_end, z_bottom, z_top, window_ys, wall_yaw_deg, label_prefix, folder):
    total_h = z_top - z_bottom
    lower_h = min(340.0, total_h * 0.70)
    upper_h = max(0.0, total_h - lower_h)
    trim_mat = LOADED_MATS.get("wood_walnut")
    wall_mat = LOADED_MATS.get("concrete_grime_tiled")

    seg_len = y_end - y_start
    seg_mid = (y_start + y_end) / 2.0
    spawn_box((x_fixed, seg_mid, z_bottom + 50.0), (16.0, seg_len, 100.0), trim_mat, f"{label_prefix}_Wainscot", folder, no_collision=True)
    spawn_box((x_fixed, seg_mid, z_top - 10.0), (22.0, seg_len, 20.0), trim_mat, f"{label_prefix}_Crown", folder, no_collision=True)

    for b_idx, (b0, _, bcy, blen) in enumerate(_subdivide_run_into_leartes_bays(y_start, y_end)):
        has_win = any(abs(bcy - wy) < blen * 0.55 for wy in (window_ys or []))
        m_key = "BLD_Wall_Window_3M" if has_win else ("BLD_Wall_3M_A" if b_idx % 2 == 0 else "BLD_Wall_3M_B")
        if not has_win:
            spawn_box((x_fixed, bcy, z_bottom + total_h / 2.0), (12.0, blen, total_h), wall_mat, f"{label_prefix}_Backer_{b_idx}", folder, no_collision=True)

        spawn_leartes_modular_span(
            m_key, (x_fixed, bcy), z_bottom, blen, lower_h, wall_yaw_deg,
            thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_Bay{b_idx}", folder=folder,
        )
        if upper_h > 10.0:
            spawn_leartes_modular_span(
                "BLD_Wall_1M", (x_fixed, bcy), z_bottom + lower_h, blen, upper_h, wall_yaw_deg,
                thickness_scale=1.15, no_collision=True, label=f"{label_prefix}_Frieze{b_idx}", folder=folder,
            )
        if b_idx > 0:
            spawn_leartes_modular_span(
                "BLD_Column_B", (x_fixed, b0), z_bottom, 22.0, total_h, wall_yaw_deg,
                no_collision=True, label=f"{label_prefix}_Pilaster{b_idx}", folder=folder,
            )


def build_ground_floor_rooms_with_leartes_props(mats):
    folder = "HorrorMansion/05_GroundFloor_LeartesRooms"

    spawn_leartes_modular_floor_grid(40.0, 2360.0, 890.0, 2160.0, 42.0, mesh_key="BLD_Floor_3M", label_prefix="EastWing1F_LeartesFloor", folder=folder)
    spawn_leartes_modular_floor_grid(40.0, 2360.0, -2160.0, -890.0, 42.0, mesh_key="BLD_Floor_3M", label_prefix="WestWing1F_LeartesFloor", folder=folder)
    spawn_leartes_modular_ceiling_grid(40.0, 1450.0, 890.0, 2160.0, 520.0, mesh_key="BLD_Ceiling_Alt_3M_B", label_prefix="Study_LeartesCeiling", folder=folder)
    spawn_leartes_modular_ceiling_grid(1450.0, 2360.0, 890.0, 2160.0, 520.0, mesh_key="BLD_Ceiling_3M", label_prefix="SecretRoom_LeartesCeiling", folder=folder)
    spawn_leartes_modular_ceiling_grid(40.0, 1150.0, -2160.0, -890.0, 520.0, mesh_key="BLD_Ceiling_3M", label_prefix="Dining_LeartesCeiling", folder=folder)
    spawn_leartes_modular_ceiling_grid(1150.0, 2360.0, -1420.0, -890.0, 520.0, mesh_key="BLD_Ceiling_1_5M", label_prefix="Corridor_LeartesCeiling", folder=folder)
    spawn_leartes_modular_ceiling_grid(1150.0, 2360.0, -2160.0, -1420.0, 520.0, mesh_key="BLD_Ceiling_Alt_3M_C", label_prefix="PuzzleRoom_LeartesCeiling", folder=folder)

    _line_exterior_room_wall_along_y(38.0, -2162.0, -885.0, 40.0, 530.0, [-1980.0, -980.0], -90.0, "Dining_FrontInnerWall", folder)
    _line_exterior_room_wall_along_x(-2162.0, 38.0, 1150.0, 40.0, 530.0, [320.0], 0.0, 760.0, "Dining_WestInnerWall", folder)
    _line_exterior_room_wall_along_x(-2162.0, 1150.0, 2362.0, 40.0, 530.0, [1680.0], 0.0, None, "PuzzleRoom_WestInnerWall", folder)
    _line_exterior_room_wall_along_y(2362.0, -2162.0, -885.0, 40.0, 530.0, [-1150.0], 90.0, "WestWing_RearInnerWall", folder)

    _line_exterior_room_wall_along_y(38.0, 885.0, 2162.0, 40.0, 530.0, [980.0, 1980.0], -90.0, "Study_FrontInnerWall", folder)
    _line_exterior_room_wall_along_x(2162.0, 38.0, 1450.0, 40.0, 530.0, [520.0], 180.0, None, "Study_EastInnerWall", folder)
    _line_exterior_room_wall_along_x(2162.0, 1450.0, 2362.0, 40.0, 530.0, [1850.0], 180.0, None, "SecretRoom_EastInnerWall", folder)
    _line_exterior_room_wall_along_y(2362.0, 885.0, 2162.0, 40.0, 530.0, [1480.0], 90.0, "SecretRoom_RearInnerWall", folder)

    build_leartes_modular_wall_along_y(
        x_fixed=1450.0, y_start=880.0, y_end=2160.0, z_bottom=40.0, z_top=530.0,
        doorways=[(1860.0, 190.0, 265.0, None, "SecretPassage_BehindBookcase")],
        label_prefix="Wall_StudyToSecretRoom", folder=folder, wall_yaw_deg=-90.0, fireplace_y=1480.0,
    )

    spawn_leartes_prop("Fireplace_Main", (1412.0, 1480.0), 42.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, scale_xyz=(1.2, 1.2, 1.2), label="Study_LeartesFireplace", folder=folder)
    spawn_leartes_prop("Fireplace_Decor_A", (1398.0, 1480.0), 44.0, yaw_deg=-90.0, no_collision=True, label="Study_FireplaceGrate", folder=folder)
    spawn_leartes_prop("Fireplace_Decor_B", (1395.0, 1530.0), 44.0, yaw_deg=-90.0, no_collision=True, label="Study_FireplaceTools", folder=folder)
    spawn_leartes_prop("Fireplace_Painting", (1422.0, 1480.0), 235.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, scale_xyz=(1.25, 1.25, 1.25), no_collision=True, label="Study_OvermantelPainting", folder=folder)
    for side, sy in [("L", 1325.0), ("R", 1635.0)]:
        spawn_leartes_prop("Sconce_Wall", (1422.0, sy), 255.0, yaw_deg=-90.0, scale_xyz=(1.2, 1.2, 1.2), no_collision=True, label=f"Study_FireplaceSconce_{side}", folder=folder)
        spawn_point_light((1385.0, sy, 265.0), (1.0, 0.72, 0.38), 320.0, 700.0, f"Light_StudySconce_{side}")

    spawn_leartes_prop("Bookcase_A", (1405.0, 1120.0), 42.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, scale_xyz=(1.15, 1.15, 1.15), label="Study_Bookcase_NorthLeft", folder=folder)
    spawn_leartes_prop("Bookcase_Cap", (1405.0, 1245.0), 42.0, yaw_deg=-90.0, no_collision=True, label="Study_BookcaseCap_NorthLeft", folder=folder)
    spawn_leartes_prop("Bookcase_B", (1388.0, 1820.0), 42.0, yaw_deg=-56.0, align_long_axis_to_yaw=True, scale_xyz=(1.1, 1.1, 1.15), no_collision=True, label="Secret_Pivoting_Bookcase_Door", folder=folder)
    for idx, bx in enumerate([320.0, 740.0, 1080.0]):
        spawn_leartes_prop("Bookcase_C" if idx % 2 == 0 else "Bookcase_D", (bx, 2115.0), 42.0, yaw_deg=180.0, align_long_axis_to_yaw=True, scale_xyz=(1.15, 1.15, 1.15), label=f"Study_Bookcase_EastWall_{idx + 1}", folder=folder)
        spawn_leartes_prop("Books_Merged_A" if idx % 2 == 0 else "Books_Merged_B", (bx, 2095.0), 125.0, yaw_deg=180.0, no_collision=True, label=f"Study_ShelfBooks_{idx + 1}", folder=folder)

    spawn_leartes_prop("OpenBook", (740.0, 2090.0), 160.0, yaw_deg=180.0, no_collision=True, label="Clue_ShelfBook_HidingSecondKey", folder=folder)
    spawn_leartes_prop("Keys_Ring", (740.0, 2086.0), 162.0, yaw_deg=45.0, no_collision=True, label="Clue_SecondKey_InsideShelfBook", folder=folder)

    spawn_leartes_prop("Carpet_SlateBlue", (920.0, 1480.0), 43.0, yaw_deg=0.0, scale_xyz=(1.4, 1.4, 1.0), no_collision=True, label="Study_SlateBlueRug", folder=folder)
    spawn_leartes_prop("Sofa_Chesterfield", (780.0, 1480.0), 42.0, yaw_deg=90.0, align_long_axis_to_yaw=True, scale_xyz=(1.15, 1.15, 1.15), label="Study_ChesterfieldSofa", folder=folder)
    spawn_leartes_prop("Blanket", (780.0, 1435.0), 105.0, yaw_deg=90.0, no_collision=True, label="Study_SofaBlanket", folder=folder)
    spawn_leartes_prop("Cushion_A", (780.0, 1535.0), 102.0, yaw_deg=75.0, no_collision=True, label="Study_SofaCushion", folder=folder)
    spawn_leartes_prop("Armchair_Wing_B", (1040.0, 1240.0), 42.0, yaw_deg=40.0, scale_xyz=(1.15, 1.15, 1.15), label="Study_WingbackArmchair_L", folder=folder)
    spawn_leartes_prop("Armchair_Wing_C", (1040.0, 1720.0), 42.0, yaw_deg=-35.0, scale_xyz=(1.15, 1.15, 1.15), label="Study_WingbackArmchair_R", folder=folder)

    _, st1_z = spawn_leartes_prop("Table_Side_B", (780.0, 1710.0), 42.0, yaw_deg=0.0, label="Study_SideTable_R", folder=folder)
    spawn_leartes_prop("Lamp_Tiffany_Green", (780.0, 1710.0), st1_z, yaw_deg=0.0, no_collision=True, label="Study_GreenTiffanyLamp_R", folder=folder)
    spawn_point_light((780.0, 1710.0, st1_z + 45.0), (0.65, 0.95, 0.62), 350.0, 750.0, "Light_StudyTiffany_R")

    _, st2_z = spawn_leartes_prop("Table_Side_A", (1120.0, 1120.0), 42.0, yaw_deg=0.0, label="Study_SideTable_L", folder=folder)
    spawn_leartes_prop("Lamp_Tiffany_Green", (1120.0, 1120.0), st2_z, yaw_deg=0.0, no_collision=True, label="Study_GreenTiffanyLamp_L", folder=folder)
    spawn_point_light((1120.0, 1120.0, st2_z + 45.0), (0.65, 0.95, 0.62), 350.0, 750.0, "Light_StudyTiffany_L")

    spawn_leartes_prop("Curtain_A", (520.0, 2138.0), 42.0, yaw_deg=180.0, align_long_axis_to_yaw=True, scale_xyz=(1.2, 1.2, 1.2), no_collision=True, label="Study_RustCurtains", folder=folder)
    spawn_leartes_prop("Frame_Leaning_A", (1220.0, 950.0), 42.0, yaw_deg=15.0, no_collision=True, label="Study_LeaningGoldFrames", folder=folder)
    spawn_leartes_prop("Books_Merged_2A", (1265.0, 985.0), 42.0, yaw_deg=25.0, no_collision=True, label="Study_FloorBooks", folder=folder)

    _, desk_z = spawn_leartes_prop("Table_Medium", (360.0, 1480.0), 42.0, yaw_deg=90.0, align_long_axis_to_yaw=True, scale_xyz=(1.35, 1.35, 1.1), label="Study_InvestigationDesk", folder=folder)
    spawn_leartes_prop("Armchair_Wing_B", (225.0, 1480.0), 42.0, yaw_deg=0.0, label="Study_DeskChair", folder=folder)
    spawn_leartes_prop("Notebook_RedInk_A", (345.0, 1395.0), desk_z, yaw_deg=14.0, no_collision=True, label="Clue_Desk_RedInkNotebook", folder=folder)
    spawn_leartes_prop("RedMarker", (375.0, 1418.0), desk_z, yaw_deg=-22.0, no_collision=True, label="Clue_Desk_RedMarkerPen", folder=folder)
    spawn_leartes_prop("Folder_A", (350.0, 1455.0), desk_z, yaw_deg=-8.0, no_collision=True, label="Clue_Desk_PropertyRecords_MaraGraves", folder=folder)
    spawn_leartes_prop("Frame_Small_C", (345.0, 1505.0), desk_z, yaw_deg=18.0, no_collision=True, label="Clue_Photo_VictorMara_ScratchedOutMila", folder=folder)
    spawn_leartes_prop("Page_B", (382.0, 1508.0), desk_z, yaw_deg=6.0, no_collision=True, label="Clue_Note_HeNeverNoticedTheSecondKey", folder=folder)
    spawn_leartes_prop("Page_C", (352.0, 1552.0), desk_z, yaw_deg=-12.0, no_collision=True, label="Clue_MarasNote_DontBelieveWhatHeTellsYou", folder=folder)
    spawn_leartes_prop("Newspaper", (385.0, 1395.0), desk_z, yaw_deg=8.0, no_collision=True, label="Clue_DeskDrawer_NewspaperClipping", folder=folder)
    spawn_leartes_prop("Envelope_A", (385.0, 1462.0), desk_z, yaw_deg=-15.0, no_collision=True, label="Clue_Desk_BlueEnvelope", folder=folder)
    spawn_leartes_prop("Papers_Group", (382.0, 1558.0), desk_z, yaw_deg=12.0, no_collision=True, label="Clue_Desk_ScatteredFamilyDocuments", folder=folder)

    spawn_leartes_prop("Bookcase_Half_A", (1950.0, 2110.0), 42.0, yaw_deg=180.0, align_long_axis_to_yaw=True, label="SecretRoom_ArchiveShelf_A", folder=folder)
    spawn_leartes_prop("Bookcase_Half_B", (1680.0, 2110.0), 42.0, yaw_deg=180.0, align_long_axis_to_yaw=True, label="SecretRoom_ArchiveShelf_B", folder=folder)
    spawn_leartes_prop("Box_National", (1750.0, 2010.0), 42.0, yaw_deg=18.0, label="SecretRoom_ArchiveBox_1", folder=folder)
    spawn_leartes_prop("Cabinet_Tall", (2305.0, 1550.0), 42.0, yaw_deg=-90.0, label="SecuritySystem_OperationalControlCabinet", folder=folder)
    spawn_leartes_prop("Cable_B", (2285.0, 1480.0), 42.0, yaw_deg=-90.0, no_collision=True, label="SecuritySystem_PowerCables_1", folder=folder)
    spawn_leartes_prop("Cable_C", (2285.0, 1620.0), 42.0, yaw_deg=-90.0, no_collision=True, label="SecuritySystem_PowerCables_2", folder=folder)
    spawn_leartes_prop("Switch_B", (2338.0, 1430.0), 160.0, yaw_deg=-90.0, no_collision=True, label="SecuritySystem_MasterBreakerSwitch", folder=folder)

    build_leartes_modular_wall_along_y(
        x_fixed=1150.0, y_start=-2160.0, y_end=-880.0, z_bottom=40.0, z_top=530.0,
        doorways=[(-1150.0, 148.0, 228.0, 85.0, "Door_DiningToFootprintCorridor")],
        label_prefix="Wall_1F_WestDivider", folder=folder, wall_yaw_deg=90.0,
    )
    build_leartes_modular_wall_along_x(
        y_fixed=-1420.0, x_start=1150.0, x_end=2360.0, z_bottom=40.0, z_top=530.0,
        doorways=[(1420.0, 148.0, 228.0, -85.0, "Door_EnterThreeDoorPuzzleRoom")],
        label_prefix="Wall_1F_HallwayToPuzzleRoom", folder=folder, wall_yaw_deg=180.0,
    )

    spawn_leartes_prop("Switch_A", (1380.0, -898.0), 155.0, yaw_deg=-90.0, no_collision=True, label="Corridor_ClickLightSwitch_1", folder=folder)
    spawn_leartes_prop("Switch_B", (1980.0, -898.0), 155.0, yaw_deg=-90.0, no_collision=True, label="Corridor_ClickLightSwitch_2", folder=folder)
    spawn_leartes_prop("Cable_D", (1680.0, -905.0), 42.0, yaw_deg=0.0, no_collision=True, label="Corridor_WallConduitCable", folder=folder)

    for fp_idx, fpx in enumerate([1320.0, 1480.0, 1640.0, 1800.0, 1960.0, 2120.0]):
        spawn_decal_plane(
            (fpx, -1150.0 + (12.0 if fp_idx % 2 == 0 else -12.0), 43.2),
            (38.0, 22.0), mats["decal_footprint"], yaw_deg=0.0,
            label=f"Corridor_DustyFootprintDecal_{fp_idx + 1}", folder=folder,
        )

    spawn_leartes_prop("Curtain_A", (2342.0, -1150.0), 42.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, scale_xyz=(1.2, 1.2, 1.2), no_collision=True, label="Corridor_EndWindowCurtains_MilaSilhouetteSightline", folder=folder)
    _, corr_tbl_z = spawn_leartes_prop("Table_Side_A", (1850.0, -960.0), 42.0, yaw_deg=0.0, label="Corridor_SideTable", folder=folder)
    spawn_leartes_prop("Lamp_Tiffany_Purple", (1850.0, -982.0), corr_tbl_z, yaw_deg=0.0, no_collision=True, label="Corridor_PurpleTiffanyLamp", folder=folder)
    spawn_leartes_prop("Vase_Blue", (1850.0, -935.0), corr_tbl_z, yaw_deg=0.0, no_collision=True, label="Corridor_BluePorcelainVase", folder=folder)
    spawn_point_light((1850.0, -982.0, corr_tbl_z + 45.0), (0.78, 0.48, 0.95), 380.0, 800.0, "Light_Corridor_PurpleTiffany")

    _, bx1_top = spawn_leartes_prop("Box_National", (1950.0, -1330.0), 42.0, yaw_deg=12.0, label="Corridor_NationalBox_1", folder=folder)
    spawn_leartes_prop("Box_National", (1952.0, -1332.0), bx1_top, yaw_deg=-15.0, label="Corridor_NationalBox_2", folder=folder)
    spawn_leartes_prop("Books_Merged_C", (2010.0, -1320.0), 42.0, yaw_deg=0.0, no_collision=True, label="Corridor_StackedBooks", folder=folder)
    spawn_leartes_prop("Frame_Leaning_C", (1750.0, -1385.0), 42.0, yaw_deg=0.0, no_collision=True, label="Corridor_LeaningGoldFrames_1", folder=folder)
    spawn_leartes_prop("Frame_Leaning_D", (1620.0, -1385.0), 42.0, yaw_deg=0.0, no_collision=True, label="Corridor_LeaningGoldFrames_2", folder=folder)
    spawn_leartes_prop("Portrait_C", (1520.0, -1398.0), 195.0, yaw_deg=90.0, scale_xyz=(1.3, 1.3, 1.3), no_collision=True, label="Clue_Corridor_DustyFamilyPhoto_CrossYouWroteItDown", folder=folder)

    for dy, d_tag in [
        (-1560.0, "ChoiceDoor_1_Left_SafeExit"),
        (-1800.0, "ChoiceDoor_2_Middle_FAKE_TRAP"),
        (-2040.0, "ChoiceDoor_3_Right_Fake"),
    ]:
        spawn_leartes_modular_span("BLD_Arch_2M", (2342.0, dy), 42.0, 165.0, 240.0, 90.0, no_collision=True, label=f"{d_tag}_LeartesArch", folder=folder)
        spawn_leartes_prop("Door", (2342.0, dy), 42.0, yaw_deg=0.0, scale_xyz=(1.08, 1.08, 1.08), no_collision=True, label=f"{d_tag}_Door", folder=folder)
    _, pzl_tbl_z = spawn_leartes_prop("Table_Side_B", (1760.0, -1800.0), 42.0, yaw_deg=90.0, label="PuzzleRoom_CenterTable", folder=folder)
    spawn_leartes_prop("Page_D", (1760.0, -1800.0), pzl_tbl_z, yaw_deg=0.0, no_collision=True, label="Clue_PuzzleRoom_MiddleOptionFactNote", folder=folder)

    spawn_leartes_prop("Fireplace_Main", (760.0, -2125.0), 42.0, yaw_deg=0.0, align_long_axis_to_yaw=True, scale_xyz=(1.2, 1.2, 1.2), label="Dining_LeartesFireplace", folder=folder)
    spawn_leartes_prop("Fireplace_Decor_A", (760.0, -2112.0), 44.0, yaw_deg=0.0, no_collision=True, label="Dining_FireplaceGrate", folder=folder)
    spawn_leartes_prop("Portrait_A", (760.0, -2115.0), 235.0, yaw_deg=-90.0, scale_xyz=(1.35, 1.35, 1.35), no_collision=True, label="Dining_OvermantelPortrait", folder=folder)
    spawn_leartes_prop("Curtain_B", (320.0, -2138.0), 42.0, yaw_deg=0.0, align_long_axis_to_yaw=True, scale_xyz=(1.2, 1.2, 1.2), no_collision=True, label="Dining_WindowCurtains", folder=folder)
    spawn_leartes_prop("Chandelier_Top", (600.0, -1540.0), 420.0, scale_xyz=(1.4, 1.4, 1.4), no_collision=True, label="Dining_Chandelier", folder=folder)
    spawn_leartes_prop("Carpet_SlateBlue", (600.0, -1540.0), 43.0, yaw_deg=90.0, scale_xyz=(1.6, 1.2, 1.0), no_collision=True, label="Dining_UnderTableRug", folder=folder)

    spawn_box((600.0, -1540.0, 116.0), (520.0, 145.0, 8.0), mats["wood_walnut_polished"], "DiningTable_Top", folder)
    spawn_decal_plane((600.0, -1540.0, 120.8), (480.0, 130.0), mats["decal_tablecloth_a"], yaw_deg=0.0, label="DiningTable_LaceTablecloth", folder=folder)
    spawn_decal_plane((545.0, -1515.0, 121.2), (35.0, 35.0), mats["decal_coffee"], yaw_deg=20.0, label="DiningTable_SpilledTeaStain", folder=folder)
    for lx in [390.0, 810.0]:
        for ly in [-1590.0, -1490.0]:
            spawn_cylinder((lx, ly, 80.0), 10.0, 72.0, mats["wood_walnut"], f"DiningTable_Leg_{int(lx)}_{int(ly)}", folder)

    for idx, cx in enumerate([440.0, 600.0, 760.0]):
        c_key = "Armchair_Wing_C" if idx % 2 == 0 else "Armchair_Wing_B"
        spawn_leartes_prop(c_key, (cx, -1690.0), 42.0, yaw_deg=90.0 + (idx - 1) * 10.0, scale_xyz=(0.92, 0.92, 0.98), label=f"DiningChair_North_{idx}", folder=folder)
        spawn_leartes_prop(c_key, (cx, -1390.0), 42.0, yaw_deg=-90.0 + (1 - idx) * 12.0, scale_xyz=(0.92, 0.92, 0.98), label=f"DiningChair_South_{idx}", folder=folder)
        spawn_leartes_prop("Plate", (cx, -1582.0), 121.0, yaw_deg=idx * 35.0, no_collision=True, label=f"DiningPlate_L_{idx}", folder=folder)
        spawn_leartes_prop("Plate", (cx, -1498.0), 121.0, yaw_deg=idx * 25.0, no_collision=True, label=f"DiningPlate_R_{idx}", folder=folder)
        spawn_leartes_prop("Cup", (cx + 28.0, -1575.0), 121.0, yaw_deg=15.0, no_collision=True, label=f"DiningCup_L_{idx}", folder=folder)
        spawn_leartes_prop("Cup", (cx - 28.0, -1505.0), 121.0, yaw_deg=-35.0, no_collision=True, label=f"DiningCup_R_{idx}", folder=folder)
    spawn_leartes_prop("Armchair_Wing_C", (285.0, -1540.0), 42.0, yaw_deg=0.0, scale_xyz=(0.95, 0.95, 1.0), label="DiningChair_HeadWest", folder=folder)
    spawn_leartes_prop("Armchair_Wing_C", (915.0, -1540.0), 42.0, yaw_deg=168.0, scale_xyz=(0.95, 0.95, 1.0), label="DiningChair_HeadEast_PulledBack", folder=folder)
    spawn_leartes_prop("Sugar", (580.0, -1540.0), 121.0, yaw_deg=0.0, no_collision=True, label="DiningTable_SugarBowl", folder=folder)
    spawn_leartes_prop("Jar_A", (635.0, -1535.0), 121.0, yaw_deg=20.0, no_collision=True, label="DiningTable_ServingJar", folder=folder)


# ==============================================================================
# 9. SECOND FLOOR & BASEMENT (`06_SecondFloor` & `07_Basement`)
# ==============================================================================

def build_second_floor_and_basement_with_leartes_props(mats):
    folder_2f = "HorrorMansion/06_SecondFloor_UpperRooms"
    folder_base = "HorrorMansion/07_Basement_Medical_HiddenChamber"

    spawn_box((1200.0, -1530.0, 528.0), (2350.0, 1290.0, 24.0), mats["wood_floor_worn_tiled"], "Floor_2F_WestWingSlab", folder_2f)
    spawn_box((1200.0, 1530.0, 528.0), (2350.0, 1290.0, 24.0), mats["wood_floor_polished_tiled"], "Floor_2F_EastWingSlab", folder_2f)
    spawn_leartes_modular_floor_grid(40.0, 2360.0, -2160.0, -890.0, 542.0, mesh_key="BLD_Floor_3M", label_prefix="WestWing2F_LeartesFloor", folder=folder_2f)
    spawn_leartes_modular_floor_grid(40.0, 2360.0, 890.0, 2160.0, 542.0, mesh_key="BLD_Floor_3M", label_prefix="EastWing2F_LeartesFloor", folder=folder_2f)
    spawn_leartes_modular_ceiling_grid(40.0, 2360.0, -2160.0, -890.0, 1020.0, mesh_key="BLD_Ceiling_3M", label_prefix="WestWing2F_LeartesCeiling", folder=folder_2f)
    spawn_leartes_modular_ceiling_grid(40.0, 2360.0, 890.0, 2160.0, 1020.0, mesh_key="BLD_Ceiling_3_5M", label_prefix="EastWing2F_LeartesCeiling", folder=folder_2f)

    _line_exterior_room_wall_along_y(38.0, -2162.0, -885.0, 540.0, 1030.0, [-1980.0, -980.0], -90.0, "VictorMaster_FrontInnerWall", folder_2f)
    _line_exterior_room_wall_along_x(-2162.0, 38.0, 1150.0, 540.0, 1030.0, [520.0], 0.0, None, "VictorMaster_WestInnerWall", folder_2f)
    _line_exterior_room_wall_along_x(-2162.0, 1150.0, 2362.0, 540.0, 1030.0, [1680.0], 0.0, None, "AtticHobby_WestInnerWall", folder_2f)
    _line_exterior_room_wall_along_y(2362.0, -2162.0, -885.0, 540.0, 1030.0, [-1480.0], 90.0, "AtticHobby_RearInnerWall", folder_2f)

    _line_exterior_room_wall_along_y(38.0, 885.0, 2162.0, 540.0, 1030.0, [980.0, 1980.0], -90.0, "MilaBedroom_FrontInnerWall", folder_2f)
    _line_exterior_room_wall_along_x(2162.0, 38.0, 1150.0, 540.0, 1030.0, [520.0], 180.0, None, "MilaBedroom_EastInnerWall", folder_2f)
    _line_exterior_room_wall_along_x(2162.0, 1150.0, 2362.0, 540.0, 1030.0, [1850.0], 180.0, None, "MaraBedroom_EastInnerWall", folder_2f)
    _line_exterior_room_wall_along_y(2362.0, 885.0, 2162.0, 540.0, 1030.0, [1480.0], 90.0, "MaraBedroom_RearInnerWall", folder_2f)

    build_leartes_modular_wall_along_y(
        x_fixed=1150.0, y_start=-2160.0, y_end=-880.0, z_bottom=540.0, z_top=1030.0,
        doorways=[(-1500.0, 148.0, 228.0, 85.0, "Door_2F_MasterToAtticHobby")],
        label_prefix="Wall_2F_WestDivider", folder=folder_2f, wall_yaw_deg=90.0,
    )
    build_leartes_modular_wall_along_y(
        x_fixed=1150.0, y_start=880.0, y_end=2160.0, z_bottom=540.0, z_top=1030.0,
        doorways=[(1500.0, 148.0, 228.0, -85.0, "Door_2F_MilaToMaraRoom")],
        label_prefix="Wall_2F_EastDivider", folder=folder_2f, wall_yaw_deg=-90.0,
    )

    spawn_leartes_prop("Carpet_SlateBlue", (620.0, 1650.0), 543.0, yaw_deg=0.0, scale_xyz=(1.3, 1.3, 1.0), no_collision=True, label="MilaBedroom_PreservedRug", folder=folder_2f)
    spawn_leartes_prop("Sofa_Chesterfield", (380.0, 1980.0), 542.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, label="MilaBedroom_PreservedBed", folder=folder_2f)
    spawn_leartes_prop("Blanket", (380.0, 1955.0), 602.0, yaw_deg=-90.0, no_collision=True, label="MilaBedroom_BedBlanket", folder=folder_2f)
    spawn_leartes_prop("Cushion_B", (325.0, 1980.0), 602.0, yaw_deg=-75.0, no_collision=True, label="MilaBedroom_BedPillow", folder=folder_2f)
    spawn_leartes_prop("Curtain_B", (520.0, 2138.0), 542.0, yaw_deg=180.0, align_long_axis_to_yaw=True, scale_xyz=(1.2, 1.2, 1.2), no_collision=True, label="MilaBedroom_WindowCurtains", folder=folder_2f)

    _, mila_d_z = spawn_leartes_prop("Table_Medium", (780.0, 1950.0), 542.0, yaw_deg=180.0, align_long_axis_to_yaw=True, scale_xyz=(1.3, 1.3, 1.05), label="MilaBedroom_FactAndPatternDesk", folder=folder_2f)
    spawn_leartes_prop("Armchair_Wing_B", (780.0, 1805.0), 542.0, yaw_deg=90.0, label="MilaBedroom_DeskChair", folder=folder_2f)

    spawn_leartes_prop("Lamp_Tiffany_Green", (690.0, 1968.0), mila_d_z, yaw_deg=0.0, no_collision=True, label="MilaBedroom_TiffanyLamp", folder=folder_2f)
    spawn_leartes_prop("Frame_Small_D", (735.0, 1928.0), mila_d_z, yaw_deg=15.0, no_collision=True, label="Clue_Photo_MaraAndMila_MyLittleSisterKnowsEverything", folder=folder_2f)
    spawn_leartes_prop("Notebook_RedInk_B", (780.0, 1932.0), mila_d_z, yaw_deg=10.0, no_collision=True, label="Clue_MarasHiddenDiary_IfHeFindsOutImDead", folder=folder_2f)
    spawn_leartes_prop("Page_F", (828.0, 1930.0), mila_d_z, yaw_deg=-18.0, no_collision=True, label="Clue_MilaFactNote_OctopusesThreeHearts", folder=folder_2f)
    spawn_leartes_prop("Page_G", (865.0, 1938.0), mila_d_z, yaw_deg=22.0, no_collision=True, label="Clue_MilaFactNote_FingerprintsRemainForYears", folder=folder_2f)
    spawn_leartes_prop("Tarot_A", (740.0, 1972.0), mila_d_z, yaw_deg=25.0, no_collision=True, label="Clue_MilaPatternCard_1", folder=folder_2f)
    spawn_leartes_prop("Tarot_B", (775.0, 1975.0), mila_d_z, yaw_deg=-12.0, no_collision=True, label="Clue_MilaPatternCard_2", folder=folder_2f)
    spawn_leartes_prop("Tarot_C", (810.0, 1972.0), mila_d_z, yaw_deg=8.0, no_collision=True, label="Clue_MilaPatternCard_3", folder=folder_2f)
    spawn_leartes_prop("Ticket_A", (848.0, 1970.0), mila_d_z, yaw_deg=-30.0, no_collision=True, label="Clue_MilaNumberClue_TicketA", folder=folder_2f)

    spawn_leartes_prop("Bookcase_A", (1085.0, 1850.0), 542.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, label="MilaBedroom_Bookshelf", folder=folder_2f)
    spawn_leartes_prop("Books_Straight_B", (1065.0, 1825.0), 665.0, yaw_deg=-90.0, no_collision=True, label="MilaBedroom_ShelfBooks", folder=folder_2f)
    spawn_leartes_prop("Folder_C", (1062.0, 1870.0), 665.0, yaw_deg=-85.0, no_collision=True, label="Clue_OfficialReport_MaraAccidentalDeath_HiddenInBooks", folder=folder_2f)
    spawn_leartes_prop("Statue_Wood_D", (1065.0, 1915.0), 665.0, yaw_deg=-90.0, no_collision=True, label="MilaBedroom_PersonalWoodenFigurine", folder=folder_2f)
    spawn_leartes_prop("Vase_Plant_A", (350.0, 1320.0), 542.0, yaw_deg=0.0, label="MilaBedroom_PreservedPlant", folder=folder_2f)

    spawn_leartes_prop("Sofa_Chesterfield", (1850.0, 1980.0), 542.0, yaw_deg=-90.0, align_long_axis_to_yaw=True, label="MaraBedroom_DustyCouchBed", folder=folder_2f)
    spawn_leartes_prop("Cloth_A", (1850.0, 1980.0), 598.0, yaw_deg=-90.0, no_collision=True, label="MaraBedroom_DustSheet", folder=folder_2f)
    spawn_leartes_prop("Cabinet_Tall", (2280.0, 1450.0), 542.0, yaw_deg=-90.0, label="MaraBedroom_Wardrobe", folder=folder_2f)
    spawn_decal_plane((1750.0, 1650.0, 543.2), (260.0, 260.0), mats["decal_grunge"], yaw_deg=15.0, label="MaraBedroom_DustGrungeDecal", folder=folder_2f)

    spawn_leartes_prop("Sofa_Chesterfield", (450.0, -1950.0), 542.0, yaw_deg=90.0, align_long_axis_to_yaw=True, label="VictorMaster_Bed", folder=folder_2f)
    spawn_leartes_prop("Cloth_B", (450.0, -1950.0), 598.0, yaw_deg=90.0, no_collision=True, label="VictorMaster_DustSheet", folder=folder_2f)
    spawn_leartes_prop("Cabinet_Tall", (950.0, -2050.0), 542.0, yaw_deg=0.0, label="VictorMaster_Cabinet", folder=folder_2f)
    spawn_leartes_prop("Lamp_Tall_Floor", (280.0, -1980.0), 542.0, yaw_deg=35.0, label="VictorMaster_FloorLamp", folder=folder_2f)

    spawn_leartes_prop("Box_National", (1650.0, -1950.0), 542.0, yaw_deg=15.0, scale_xyz=(1.4, 1.4, 1.4), label="AtticHobby_ArchiveBox_1", folder=folder_2f)
    spawn_leartes_prop("Frame_Leaning_B", (1880.0, -1980.0), 542.0, yaw_deg=25.0, no_collision=True, label="AtticHobby_LeaningFrames_1", folder=folder_2f)
    spawn_leartes_prop("Frame_Leaning_E", (2020.0, -1960.0), 542.0, yaw_deg=-15.0, no_collision=True, label="AtticHobby_LeaningFrames_2", folder=folder_2f)

    spawn_box((1730.0, 0.0, -310.0), (1280.0, 1780.0, 20.0), mats["concrete_tiles_tiled"], "Basement_FloorSlab", folder_base)
    spawn_box((1100.0, 0.0, -150.0), (24.0, 1780.0, 300.0), mats["stone_hewn_tiled"], "Basement_Wall_Front", folder_base)
    spawn_box((2360.0, 0.0, -150.0), (24.0, 1780.0, 300.0), mats["stone_hewn_tiled"], "Basement_Wall_Rear", folder_base)
    spawn_box((1730.0, -880.0, -150.0), (1280.0, 24.0, 300.0), mats["stone_hewn_tiled"], "Basement_Wall_West", folder_base)
    spawn_box((1730.0, 880.0, -150.0), (1280.0, 24.0, 300.0), mats["stone_hewn_tiled"], "Basement_Wall_East", folder_base)

    for s in range(18):
        sx = 1830.0 + s * 28.0
        sz = 25.0 - s * 18.0
        spawn_box((sx, -500.0, sz), (34.0, 180.0, 18.0), mats["stone_cut_tiled"], f"Basement_Stair_Step_{s}", folder_base)

    build_leartes_modular_wall_along_x(
        y_fixed=-180.0, x_start=1110.0, x_end=2350.0, z_bottom=-300.0, z_top=0.0,
        doorways=[(1950.0, 148.0, 228.0, 85.0, "Door_ConcealedMedicalRoom")],
        label_prefix="BasementWall_ToMedicalRoom", folder=folder_base, wall_yaw_deg=0.0,
    )
    build_leartes_modular_wall_along_x(
        y_fixed=360.0, x_start=1110.0, x_end=2350.0, z_bottom=-300.0, z_top=0.0,
        doorways=[(1650.0, 148.0, 228.0, -85.0, "Door_MilasHiddenChamber")],
        label_prefix="BasementWall_ToHiddenChamber", folder=folder_base, wall_yaw_deg=0.0,
    )

    _, med_t_z = spawn_leartes_prop("Table_Medium", (1650.0, 80.0), -300.0, yaw_deg=0.0, scale_xyz=(1.4, 1.1, 1.1), label="MedicalRoom_ExamTable", folder=folder_base)
    spawn_leartes_prop("Jar_A", (1615.0, 65.0), med_t_z, yaw_deg=15.0, no_collision=True, label="MedicalRoom_SpecimenJar_A", folder=folder_base)
    spawn_leartes_prop("Jar_B", (1675.0, 90.0), med_t_z, yaw_deg=-25.0, no_collision=True, label="MedicalRoom_SpecimenJar_B", folder=folder_base)
    spawn_leartes_prop("Cabinet_Tall", (1160.0, 80.0), -300.0, yaw_deg=0.0, label="MedicalRoom_MedicineCabinet", folder=folder_base)
    spawn_leartes_prop("Cable_E", (1450.0, 120.0), -300.0, yaw_deg=30.0, no_collision=True, label="MedicalRoom_FloorCables", folder=folder_base)

    spawn_leartes_prop("Armchair_Wing_C", (1660.0, 620.0), -300.0, yaw_deg=25.0, roll_deg=22.0, scale_xyz=(0.9, 0.9, 0.9), label="HiddenChamber_BrokenChair", folder=folder_base)
    _, ch_tbl_z = spawn_leartes_prop("Table_Medium", (1400.0, 640.0), -300.0, yaw_deg=0.0, label="HiddenChamber_BelongingsTable", folder=folder_base)
    spawn_leartes_prop("Page_H", (1400.0, 640.0), ch_tbl_z, yaw_deg=12.0, no_collision=True, label="Clue_MilaFinalPage_PleaseDontLetHimWin", folder=folder_base)
    spawn_leartes_prop("Statue_Wood_A", (1355.0, 625.0), ch_tbl_z, yaw_deg=-20.0, no_collision=True, label="HiddenChamber_ChildToyFigurine", folder=folder_base)
    spawn_leartes_prop("Frame_Small_D", (1445.0, 655.0), ch_tbl_z, yaw_deg=30.0, no_collision=True, label="HiddenChamber_SistersPhotograph", folder=folder_base)
    spawn_leartes_prop("Books_Merged_Small", (1340.0, 740.0), -300.0, yaw_deg=18.0, no_collision=True, label="HiddenChamber_MilaChildhoodBooks", folder=folder_base)
    spawn_leartes_prop("Cushion_C", (1920.0, 710.0), -300.0, yaw_deg=-15.0, no_collision=True, label="HiddenChamber_FloorCushion", folder=folder_base)
    spawn_box((2180.0, 680.0, -275.0), (140.0, 220.0, 45.0), mats["stone_cut_tiled"], "HiddenChamber_MilaRemainsAltar", folder_base)
    spawn_leartes_prop("Blanket", (2180.0, 680.0), -252.0, yaw_deg=0.0, no_collision=True, label="HiddenChamber_RemainsShroud", folder=folder_base)


# ==============================================================================
# 10. LIGHTING & MAIN ENTRY POINT
# ==============================================================================

def build_story_lights():
    folder = "HorrorMansion/08_Lighting"

    spawn_point_light((-1250.0, 0.0, 320.0), (0.60, 0.76, 0.98), 1200.0, 1900.0, "Light_Forecourt_FountainFill", folder)
    spawn_point_light((-240.0, 0.0, 440.0), (1.0, 0.68, 0.32), 950.0, 1400.0, "Light_Portico_Entrance", folder)

    spawn_point_light((680.0, 0.0, 760.0), (1.0, 0.70, 0.36), 1450.0, 2200.0, "Light_Foyer_ChandelierFill", folder)
    spawn_point_light((760.0, 435.0, 165.0), (0.65, 0.95, 0.62), 450.0, 900.0, "Light_Foyer_SmallStairLamp", folder)
    spawn_spot_light((680.0, 0.0, 750.0), (-90.0, 0.0, 0.0), (1.0, 0.74, 0.40), 1800.0, 1400.0, 22.0, 48.0, 0.6, "SpotLight_Foyer_FloorPool", folder)
    spawn_point_light((1740.0, 0.0, 420.0), (1.0, 0.16, 0.10), 680.0, 950.0, "Light_StairLanding_RedSconce", folder)

    spawn_point_light((600.0, -1540.0, 380.0), (1.0, 0.68, 0.34), 1150.0, 1600.0, "Light_1F_DiningRoom_Fill", folder)
    spawn_point_light((1450.0, -1150.0, 440.0), (0.95, 0.74, 0.45), 750.0, 1200.0, "Light_1F_Corridor_Click_1", folder)
    spawn_point_light((2050.0, -1150.0, 440.0), (0.95, 0.74, 0.45), 750.0, 1200.0, "Light_1F_Corridor_Click_2", folder)
    spawn_spot_light((2340.0, -1150.0, 310.0), (-25.0, 180.0, 0.0), (0.85, 0.88, 0.95), 2400.0, 1800.0, 20.0, 42.0, 1.5, "SpotLight_Corridor_EndWindowPool", folder)
    spawn_point_light((1800.0, -1800.0, 420.0), (0.95, 0.64, 0.36), 1100.0, 1500.0, "Light_1F_ThreeDoorPuzzle_Fill", folder)
    spawn_point_light((780.0, 1480.0, 380.0), (1.0, 0.74, 0.42), 1250.0, 1700.0, "Light_1F_PrivateStudy_Fill", folder)
    spawn_point_light((1950.0, 1550.0, 380.0), (0.85, 0.76, 0.55), 850.0, 1400.0, "Light_1F_SecretBackRoom_Fill", folder)

    spawn_point_light((680.0, 1600.0, 880.0), (1.0, 0.76, 0.46), 1250.0, 1650.0, "Light_2F_MilaBedroom_Fill", folder)
    spawn_point_light((1800.0, 1600.0, 880.0), (0.70, 0.80, 0.95), 950.0, 1550.0, "Light_2F_MaraBedroom_Fill", folder)
    spawn_point_light((600.0, -1600.0, 880.0), (1.0, 0.68, 0.36), 950.0, 1550.0, "Light_2F_VictorMaster_Fill", folder)
    spawn_point_light((1800.0, -1600.0, 880.0), (0.95, 0.64, 0.34), 950.0, 1550.0, "Light_2F_AtticHobby_Fill", folder)
    spawn_point_light((1950.0, -500.0, -60.0), (0.75, 0.82, 0.95), 750.0, 1200.0, "Light_Basement_Stairs", folder)
    spawn_point_light((1650.0, 80.0, -45.0), (0.68, 0.92, 0.86), 1100.0, 1500.0, "Light_Basement_MedicalRoom", folder)
    spawn_point_light((1750.0, 620.0, -45.0), (0.85, 0.78, 0.92), 950.0, 1400.0, "Light_Basement_HiddenChamber", folder)


def generate_horror_mansion_environment():
    with unreal.ScopedEditorTransaction("Build Did You Know - V8 AAA Definitive Estate & 4K Leartes Modular Mansion") as _:
        unreal.log("================================================================")
        unreal.log("Building 'DID YOU KNOW?' V8 AAA Definitive Edition...")
        cleanup_previous_mansion_actors()
        mats = load_starter_and_tiled_materials()
        build_atmosphere()
        build_outer_mafia_boss_estate(mats)
        build_mansion_exterior(mats)
        build_grand_foyer(mats)
        build_ground_floor_rooms_with_leartes_props(mats)
        build_second_floor_and_basement_with_leartes_props(mats)
        build_story_lights()
        unreal.log("V8 AAA Definitive Estate & 4K Leartes Modular Mansion built! Press Alt+P to explore.")
        unreal.log("================================================================")


if __name__ == "__main__":
    generate_horror_mansion_environment()