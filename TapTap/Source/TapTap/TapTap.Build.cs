// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TapTap : ModuleRules
{
	public TapTap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {

			});
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			});
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "TapCommon",
                "TapLogin",
                "AntiAddiction"
            });
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			});

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
        }
    }
}
