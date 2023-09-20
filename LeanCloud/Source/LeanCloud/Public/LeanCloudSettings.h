#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "LeanCloudSettings.generated.h"

USTRUCT()
struct LEANCLOUD_API FLCAppConfigSettings
{
	GENERATED_BODY()

	UPROPERTY(config, EditAnywhere, Category=Config, meta = (DisplayName = "HTTP Request Timeout Interval"))
	double HTTPRequestTimeoutInterval = 60;

	UPROPERTY(config, EditAnywhere, Category=Config)
	bool bIsProduction = true;
	
};

USTRUCT()
struct LEANCLOUD_API FLCApplicationSettings
{
	GENERATED_BODY()

	UPROPERTY(config, EditAnywhere, Category=Application)
	FString AppId;

	UPROPERTY(config, EditAnywhere, Category=Application)
	FString AppKey;
	
	UPROPERTY(config, EditAnywhere, Category=Application)
	FString ServerUrl;

	UPROPERTY(config, EditAnywhere, Category=Application)
	FLCAppConfigSettings Config;

	FLCApplicationSettings() = default;
	FLCApplicationSettings(const FString& InAppId, const FString& InAppKey, const FString& InServerUrl = "");
};

/**
* Implements the settings for the LeanCloud plugin.
*/
UCLASS(Config = Engine, DefaultConfig)
class LEANCLOUD_API ULeanCloudSettings : public UObject
{
	GENERATED_UCLASS_BODY()

	/** List of Application for LeanCloud. */
	UPROPERTY(config, EditAnywhere)
	TArray<FLCApplicationSettings> Applications;
};
