// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HappyBearVillage : ModuleRules
{
	public HappyBearVillage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.AddRange(new string[] { "HappyBearVillage" });
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"OnlineSubsystem",
            "OnlineSubsystemSteam",
			"Core",
			"UMG",
			"MultiplayerSessions"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime", "PCG" });

        PublicIncludePaths.AddRange(new string[] { "HappyBearVillage" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
