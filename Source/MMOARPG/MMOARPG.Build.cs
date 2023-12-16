// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMOARPG : ModuleRules
{
	public MMOARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"SimpleThread",
			"SimpleNetChannel",		
			"MMOARPGCommon",
            "Slate",
            "SlateCore",
            "SimpleActorBrowsing",
            });

        //bEnableShadowVariableWarnings = false;
        bEnableUndefinedIdentifierWarnings = false;
        bEnableExceptions = true;

        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{
        //    Definitions.Add("_CRT_SECURE_NO_WARNINGS");
        //}
    }
}
