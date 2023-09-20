// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;
public class TapBootstrap : ModuleRules
{
	public TapBootstrap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        FileReference fileRef = new FileReference(Path.Combine(PluginDirectory, Name + ".uplugin"));
        PluginInfo plugin = new PluginInfo(fileRef, PluginType.Project);	
        PublicDefinitions.Add(Name + "_UE_VERSION_NUMBER=TEXT(\"" + plugin.Descriptor.Version + "\")");
        PublicDefinitions.Add(Name + "_UE_VERSION=TEXT(\"" + plugin.Descriptor.VersionName + "\")");


		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "ApplicationCore",
                "UMG",
                "Json",
                "JsonUtilities",
                "TapCommon",
                "TapLogin",
                "HTTP"
				// ... add other public dependencies that you statically link with here ...
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
                // ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
        

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicAdditionalFrameworks.Add(
                new Framework(
                    "TapBootstrapSDK",
                    "../ThirdParty/iOS/Frameworks/TapBootstrapSDK.zip"
                )
            );
            PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/IOS")));
            PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/iOS")));
        }

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                        "Launch"
                }
            );
            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                Path.Combine(ModuleDirectory, "TapBootstrap_Android_UPL.xml")
            );
            PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/Android")));
            PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Android")));
        }

    }
}

