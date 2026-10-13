// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DidYouKnow : ModuleRules
{
	public DidYouKnow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"DidYouKnow",
			"DidYouKnow/Variant_Horror",
			"DidYouKnow/Variant_Horror/UI",
			"DidYouKnow/Variant_Horror/Interaction",
			"DidYouKnow/Variant_Horror/Investigation"
		});
	}
}