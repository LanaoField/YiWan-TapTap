// Copyright 2022 CQUnreal. All Rights Reserved.

#include "TapTapModule.h"
#include "TapUECommon.h"
#include "TapUELogin.h"


#define LOCTEXT_NAMESPACE "FTapTapModule"

static bool bStartupAntiAddiction = false;
static bool bIsInitializedTapTapSDK = false;

void FTapTapModule::StartupModule()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FTapTapModule::HandleAppSuspend);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FTapTapModule::HandleAppResume);
	FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &FTapTapModule::WillDeactivate);
	FCoreDelegates::ApplicationHasReactivatedDelegate.AddRaw(this, &FTapTapModule::HasReactivated);
}

void FTapTapModule::ShutdownModule()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::LeaveGame();
		AntiAddictionUE::Exit();
	}

	bIsInitializedTapTapSDK = false;
	bStartupAntiAddiction = false;
}

void FTapTapModule::Init() const
{
	if (!bIsInitializedTapTapSDK)
	{
		bIsInitializedTapTapSDK = true;

		FString ClientID;
		GConfig->GetString(TEXT("/Script/TapTap.TapTapSettings"), TEXT("ClientID"), ClientID, GGameIni);

		FString RegionTypeStr;
		GConfig->GetString(TEXT("/Script/TapTap.TapTapSettings"), TEXT("RegionType"), RegionTypeStr, GGameIni);
		UEnum* RegionTypeEnum = StaticEnum<ERegionType>();
		ERegionType RegionType = (ERegionType)RegionTypeEnum->GetValueByName(*RegionTypeStr);

		FString LanguageTypeStr;
		GConfig->GetString(TEXT("/Script/TapTap.TapTapSettings"), TEXT("LanguageType"), LanguageTypeStr, GGameIni);
		UEnum* LanguageTypeEnum = StaticEnum<ELanguageType>();
		ELanguageType LanguageType = (ELanguageType)RegionTypeEnum->GetValueByName(*RegionTypeStr);

		FString ClientToken;
		GConfig->GetString(TEXT("/Script/TapTap.TapTapSettings"), TEXT("ClientToken"), ClientToken, GGameIni);

		FString ServerURL;
		GConfig->GetString(TEXT("/Script/TapTap.TapTapSettings"), TEXT("ServerURL"), ServerURL, GGameIni);

		TapUECommon::SetLanguage(LanguageType);

		// 初始化公共模块
		FTUConfig TUConfig;
		TUConfig.ClientID = ClientID;
		TUConfig.RegionType = RegionType;
		TUConfig.ClientToken = ClientToken;
		TUConfig.ServerURL = ServerURL;
		FTUConfig::Init(TUConfig);

		// 初始化登录模块
		FTULoginConfig TULoginConfig;
		TULoginConfig.ClientID = ClientID;
		TULoginConfig.RegionType = RegionType;
		TULoginConfig.bRoundCorner = true;
		TapUELogin::Init(TULoginConfig);

		// 初始化反沉迷模块
		FAAUConfig AAUConfig;
		AAUConfig.ClientID = ClientID;
		AAUConfig.UseTapLogin = false;			// 暂不支持
		AAUConfig.ShowSwitchAccount = false;
		AAUConfig.Region = EAAURegion::China;
		AntiAddictionUE::Init(AAUConfig);
	}
}

void FTapTapModule::Login(TFunction<void(const TUAuthResult& Result)> CallBack) const
{
	TSharedPtr<FTUAccessToken> TUAccessToken = TapUELogin::GetAccessToken();
	if (TUAccessToken.IsValid())
	{
		if (CallBack)
		{
			CallBack(TUAuthResult::SuccessInit(TUAccessToken));
		}
	}
	else
	{
		TapUELogin::Login(CallBack);
	}
}

void FTapTapModule::Login(const TArray<FString>& Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) const
{
	TapUELogin::Login(Permissions, CallBack);
}

void FTapTapModule::Logout() const
{
	TapUELogin::Logout();
}

TSharedPtr<FTULoginProfileModel> FTapTapModule::GetProfile() const
{
	return TapUELogin::GetProfile();
}

void FTapTapModule::ListenAntiAddictionCallback(const AntiAddictionUE::FCallBack& CallBack) const
{
	AntiAddictionUE::OnCallBack = CallBack;
}

void FTapTapModule::StartupAntiAddiction(const FString& UserID, bool bIsTapUser) const
{
	AntiAddictionUE::Startup(UserID, bIsTapUser);
}

int32 FTapTapModule::GetAntiAddictionAgeRange() const
{
	return (int32)AntiAddictionUE::GetAgeRange();
}

FString FTapTapModule::GetAntiAddictionToken() const
{
	return AntiAddictionUE::CurrentToken();
}

int32 FTapTapModule::GetAntiAddictionRemainingTime() const
{
	return AntiAddictionUE::GetRemainingTime();
}

void FTapTapModule::CheckAntiAddictionPayLimit(int32 Amount, TFunction<void(bool bStatus)> CallBack, TFunction<void(const FString& Message)> FailureHandler) const
{
	AntiAddictionUE::CheckPayLimit(Amount, CallBack, FailureHandler);
}

void FTapTapModule::SubmitAntiAddictionPayResult(int32 Amount, TFunction<void(bool bSuccess)> CallBack, TFunction<void(const FString& Message)> FailureHandler) const
{
	AntiAddictionUE::SubmitPayResult(Amount, CallBack, FailureHandler);
}

void FTapTapModule::TestAntiAddiction() const
{
	AntiAddictionUE::Test();
}

void FTapTapModule::WillDeactivate()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::LeaveGame();
	}
}

void FTapTapModule::HasReactivated()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::EnterGame();
	}
}

void FTapTapModule::HandleAppSuspend()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::LeaveGame();
	}
}

void FTapTapModule::HandleAppResume()
{
	if (bStartupAntiAddiction)
	{
		AntiAddictionUE::EnterGame();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTapTapModule, TapTap)