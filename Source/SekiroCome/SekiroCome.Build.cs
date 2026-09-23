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

		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		PublicIncludePaths.AddRange(new string[] {
			"SekiroCome",
			"SekiroCome/Variant_Combat",
			"SekiroCome/Variant_Combat/AI",
			"SekiroCome/Variant_Combat/Animation",
			"SekiroCome/Variant_Combat/Gameplay",
			"SekiroCome/Variant_Combat/Interfaces",
			"SekiroCome/Variant_Combat/UI",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
