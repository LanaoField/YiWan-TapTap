// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TUAuthResult.h"
#include "AntiAddictionUE.h"
#include "TULoginProfileModel.h"
#include "Modules/ModuleManager.h"


class FTapTapModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void Init() const;
	void Login(TFunction<void(const TUAuthResult& Result)> CallBack) const;
	void Login(const TArray<FString>& Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) const;
	void Logout() const;
	TSharedPtr<FTULoginProfileModel> GetProfile() const;

	// ·À³ÁÃÔ½Ó¿Ú
	void ListenAntiAddictionCallback(const AntiAddictionUE::FCallBack& CallBack) const;
	void StartupAntiAddiction(const FString& UserID, bool bIsTapUser = false) const;
	int32 GetAntiAddictionAgeRange() const;
	FString GetAntiAddictionToken() const;
	int32 GetAntiAddictionRemainingTime() const;
	void CheckAntiAddictionPayLimit(int32 Amount, TFunction<void(bool bStatus)> CallBack, TFunction<void(const FString& Message)> FailureHandler) const;
	void SubmitAntiAddictionPayResult(int32 Amount, TFunction<void(bool bSuccess)> CallBack, TFunction<void(const FString& Message)> FailureHandler) const;
	void TestAntiAddiction() const;

	static inline FTapTapModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FTapTapModule>("TapTap");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("TapTap");
	}

protected:
	// This is called when the application is about to be deactivated (e.g., due to a phone call or SMS or the sleep button).
	void WillDeactivate();

	// Called when the application has been reactivated (reverse any processing done in the Deactivate delegate)
	void HasReactivated();

	// Callback occurs when game being suspended
	void HandleAppSuspend();

	// Callback occurs when game resuming
	void HandleAppResume();
};
