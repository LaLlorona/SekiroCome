// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SekiroCome : ModuleRules
{
	public SekiroCome(ReadOnlyTargetRules Target) : base(Target)
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
			"SekiroCome",
			"SekiroCome/Variant_Platforming",
			"SekiroCome/Variant_Platforming/Animation",
			"SekiroCome/Variant_Combat",
			"SekiroCome/Variant_Combat/AI",
			"SekiroCome/Variant_Combat/Animation",
			"SekiroCome/Variant_Combat/Gameplay",
			"SekiroCome/Variant_Combat/Interfaces",
			"SekiroCome/Variant_Combat/UI",
			"SekiroCome/Variant_SideScrolling",
			"SekiroCome/Variant_SideScrolling/AI",
			"SekiroCome/Variant_SideScrolling/Gameplay",
			"SekiroCome/Variant_SideScrolling/Interfaces",
			"SekiroCome/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
