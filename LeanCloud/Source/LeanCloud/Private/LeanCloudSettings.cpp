#include "LeanCloudSettings.h"


ULeanCloudSettings::ULeanCloudSettings(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
		, Applications(TArray<FLCApplicationSettings>())
{
}

FLCApplicationSettings::FLCApplicationSettings(const FString& InAppId, const FString& InAppKey,
	const FString& InServerUrl) : AppId(InAppId), AppKey(InAppKey), ServerUrl(InServerUrl) {
	Config = FLCAppConfigSettings();
}
