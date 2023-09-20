// Some copyright should be here...

using System;
using UnrealBuildTool;
using System.IO;
public class LeanCloud : ModuleRules
{
	public LeanCloud(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Foundation")));
		PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/RTM")));
		
		Console.WriteLine("LeanCloud Module Load");

		bEnableExceptions = true;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Json", "JsonUtilities",
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"JsonUtilities",
				"Http",
				"CoreUObject",
				// "Engine",
				// "Slate",
				// "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
		);
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Settings",
				}
			);
		}
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}