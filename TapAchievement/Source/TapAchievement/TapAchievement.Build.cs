// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;

public class TapAchievement : ModuleRules
{
	public TapAchievement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		FileReference fileRef = new FileReference(Path.Combine(PluginDirectory, Name + ".uplugin"));
		PluginInfo plugin = new PluginInfo(fileRef, PluginType.Project);	
		PublicDefinitions.Add(Name + "_UE_VERSION_NUMBER=TEXT(\"" + plugin.Descriptor.Version + "\")");
		PublicDefinitions.Add(Name + "_UE_VERSION=TEXT(\"" + plugin.Descriptor.VersionName + "\")");


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"TapCommon",
				"TapBootstrap"
			}
		);
		
		if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"LeanCloudMobile"
				}
			);
		}
		else
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"LeanCloud"
				}
			);
		}
		
		

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Json",
				"JsonUtilities",
				"HTTP",
				"UMG",
				"ImageWrapper",
			}
		);
	}
}