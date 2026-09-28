// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LavaGuayaraZhang : ModuleRules
{
	public LavaGuayaraZhang(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LavaGuayaraZhang",
			"LavaGuayaraZhang/Variant_Platforming",
			"LavaGuayaraZhang/Variant_Platforming/Animation",
			"LavaGuayaraZhang/Variant_Combat",
			"LavaGuayaraZhang/Variant_Combat/AI",
			"LavaGuayaraZhang/Variant_Combat/Animation",
			"LavaGuayaraZhang/Variant_Combat/Gameplay",
			"LavaGuayaraZhang/Variant_Combat/Interfaces",
			"LavaGuayaraZhang/Variant_Combat/UI",
			"LavaGuayaraZhang/Variant_SideScrolling",
			"LavaGuayaraZhang/Variant_SideScrolling/AI",
			"LavaGuayaraZhang/Variant_SideScrolling/Gameplay",
			"LavaGuayaraZhang/Variant_SideScrolling/Interfaces",
			"LavaGuayaraZhang/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
