// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Network_TP1 : ModuleRules
{
	public Network_TP1(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Network_TP1",
			"Network_TP1/Variant_Platforming",
			"Network_TP1/Variant_Combat",
			"Network_TP1/Variant_Combat/AI",
			"Network_TP1/Variant_SideScrolling",
			"Network_TP1/Variant_SideScrolling/Gameplay",
			"Network_TP1/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
