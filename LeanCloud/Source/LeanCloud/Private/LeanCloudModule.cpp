#include "LeanCloudModule.h"
#include "LCApplication.h"
#include "LeanCloudSettings.h"
#if WITH_EDITOR
#include "Developer/Settings/Public/ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FLeanCloudModule"

void FLeanCloudModule::StartupModule() {
	// register settings
#if WITH_EDITOR
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr) {
		SettingsModule->RegisterSettings("Project", "Plugins", "LeanCloud",
		                                 LOCTEXT("LeanCloudSettingsName", "Lean Cloud"),
		                                 LOCTEXT("LeanCloudSettingsDescription",
		                                         "Project settings for LeanCloud plugin"),
		                                 GetMutableDefault<ULeanCloudSettings>()
		);
	}
#endif

	for (auto Application : GetDefault<ULeanCloudSettings>()->Applications) {
		FLCApplication::Register(Application);
	}
}

void FLeanCloudModule::ShutdownModule() {
	// unregister settings
#if WITH_EDITOR
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr) {
		SettingsModule->UnregisterSettings("Project", "Plugins", "LeanCloud");
	}
#endif

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLeanCloudModule, LeanCloud)
