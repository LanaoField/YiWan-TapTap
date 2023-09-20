#include "LCApplication.h"

#include "LCUser.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Network/LCHttpClient.h"
#include "Network/LCAppRouter.h"

#include "Tools/LCDebuger.h"
#include "Tools/LCJsonHelper.h"

static FString CurrentUserFileName = ".userdata";

TMap<FString, TSharedPtr<FLCApplication>> FLCApplication::Registry;
TSharedPtr<FLCApplication> FLCApplication::DefaultPtr;

void FLCApplication::SetLogDelegate(FLeanCloudLogDelegate LogDelegate) {
	FLCDebuger::LogDelegate = LogDelegate;
}

const FString& FLCApplication::GetAppId() const {
	return Settings.AppId;
}

const FString& FLCApplication::GetAppKey() const {
	return Settings.AppKey;
}

const FString& FLCApplication::GetServerUrl() const {
	return Settings.ServerUrl;
}

const FLCAppConfigSettings& FLCApplication::GetConfig() const {
	return Settings.Config;
}

TSharedPtr<FLCApplication> FLCApplication::Register(const FLCApplicationSettings& InSettings) {
	if (InSettings.AppId.IsEmpty()) {
		return nullptr;
	}
	TSharedPtr<FLCApplication> Ptr = MakeShared<FLCApplication>(InSettings);
	Ptr->HttpClient = MakeShared<FLCHttpClient>(Ptr);
	Ptr->AppRouter = MakeShared<FLCAppRouter>(Ptr);
	Ptr->LoadCurrentUserFromLocal();

	Registry.Add(InSettings.AppId, Ptr);
	if (!DefaultPtr.IsValid()) {
		DefaultPtr = Ptr;
	}
	return Ptr;
}

TSharedPtr<FLCApplication> FLCApplication::Get(const FString& InAppId) {
	TSharedPtr<FLCApplication> * Ptr = Registry.Find(InAppId);
	if (Ptr == nullptr) {
		return nullptr;
	} else {
		return *Ptr;
	}
}

void FLCApplication::Unregister() {
	Registry.Remove(Settings.AppId);
}

FLCApplication::FLCApplication() {
}

void FLCApplication::LoadCurrentUserFromLocal() {
	TArray<uint8> Data;
	if (AppRouter->LoadFile(CurrentUserFileName, Data) && Data.Num() > 0) {
		FString UserStr = FLCHelper::UTF8Encode(Data);
		TLCMap UserMap = FLCJsonHelper::GetJsonValue(UserStr).AsMap();
		CurrentUser = FLCObject::CreateObject<FLCUser>(UserMap, AsShared());
	} else {
		// 兼容老TDSUser
		const FString FilePath = FPaths::SandboxesDir() + TEXT("/DataStorage/TDSUser");
		if (FPaths::FileExists(FilePath))
		{
			if (FFileHelper::LoadFileToArray(Data, *FilePath))
			{
				const FString UserStr = FLCHelper::UTF8Encode(FLCHelper::AesDecode(Data, FLCHelper::UTF8Encode(FString("tap_save_file_key"))));
				TLCMap UserMap = FLCJsonHelper::GetJsonValue(UserStr).AsMap();
				SetCurrentUser(FLCObject::CreateObject<FLCUser>(UserMap, AsShared()));
				FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath);
			}
		}
	}
}

FLCApplication::FLCApplication(const FLCApplicationSettings& InSettings): Settings(InSettings) {
	
}


TSharedPtr<FLCUser> FLCApplication::GetCurrentUser() {
	return CurrentUser;
}

void FLCApplication::SetCurrentUser(const TSharedPtr<FLCUser>& InUser) {
	CurrentUser = InUser;
	TArray<uint8> Data;
	if (InUser.IsValid()) {
		Data = FLCHelper::UTF8Encode(InUser->ToString());
	}
	AppRouter->SaveFile(CurrentUserFileName, Data);
}
