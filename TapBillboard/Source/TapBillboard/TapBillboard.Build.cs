// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;
public class TapBillboard : ModuleRules
{
	public TapBillboard(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		FileReference fileRef = new FileReference(Path.Combine(PluginDirectory, Name + ".uplugin"));
		PluginInfo plugin = new PluginInfo(fileRef, PluginType.Project);	
		PublicDefinitions.Add(Name + "_UE_VERSION_NUMBER=TEXT(\"" + plugin.Descriptor.Version + "\")");
		PublicDefinitions.Add(Name + "_UE_VERSION=TEXT(\"" + plugin.Descriptor.VersionName + "\")");


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine", 
				"HTTP",
				"TapCommon",
				"TapBootstrap",
				"Json",
				"JsonUtilities",
				"UMG",
				"SlateCore",
				"Slate",
				"WebBrowser"
			}
		);

		PrivateIncludePaths.Add("TapBillboard/Private");
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicFrameworks.Add("UIKit");
			PrivateIncludePaths.Add("TapBillboard/Private/IOS");
			PublicAdditionalFrameworks.Add(
				new Framework(
					"TapBillboardSDK",
					"../ThirdParty/iOS/Frameworks/TapBillboardSDK.zip",
					"TapBillboardResource.bundle"
				)
			);
		}
		else if(Target.Platform == UnrealTargetPlatform.Android)
		{
			AdditionalPropertiesForReceipt.Add("AndroidPlugin",Path.Combine(ModuleDirectory, "TapBillboard_Android_UPL.xml"));
			PrivateDependencyModuleNames.Add("Launch");
			PrivateIncludePaths.Add("TapBillboard/Private/Android");
		}
		else
		{
			PrivateIncludePaths.Add("TapBillboard/Private/PC");
		}
	}
}