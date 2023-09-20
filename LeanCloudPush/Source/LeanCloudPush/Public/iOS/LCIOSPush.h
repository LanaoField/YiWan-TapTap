#pragma once
#include "LeanCloudPush.h"

class LEANCLOUDPUSH_API FLCIOSPush : FLeanCloudPush {
public:
	static void Register(const FString& InTeamID);

	static FString GetInstallationObjectID();

	static FString GetInstallationInstallationId();

	static FString GetInstallationDeviceToken();
	// virtual void SubscribeChannel(const FString& ChannelName) override;
	// virtual void UnsubscribeChannel(const FString& ChannelName) override;
	//
	//
	// static void PushUserInfo(NSDictionary* userInfo);
};
