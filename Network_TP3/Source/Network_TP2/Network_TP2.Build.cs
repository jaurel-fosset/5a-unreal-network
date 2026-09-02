// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Network_TP2 : ModuleRules
{
	public Network_TP2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" });
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemSteam" });
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
