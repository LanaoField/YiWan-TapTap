// Copyright 2022 CQUnreal. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TUError.h"
#include "TUAccessToken.h"
#include "AAUType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TapTapBlueprintLibrary.generated.h"

UENUM(BlueprintType)
namespace ETapTapLogin
{
	enum Type
	{
		Success,
		Cancel,
		Fail,
	};
}

UENUM(BlueprintType)
namespace ETapTapAntiAddiction
{
	enum Type
	{
		None = 0,
		LoginSuccess = 500,			// 登录成功
		Exited = 1000,				// 退出登录
		DurationLimit = 1050,		// 可玩时长耗尽
		PeriodRestrict = 1030,		// 达到宵禁时长
		RealNameStop = 9002,		// 实名过程中点击了关闭实名窗
		SwitchAccount = 1001		// 切换账号
	};
}

USTRUCT(BlueprintType)
struct FTapTapAccessToken
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString KID;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString AccessToken;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString TokenType;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString MacKey;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString MacAlgorithm;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	int64 Scope;
};

USTRUCT(BlueprintType)
struct FTapTapError
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	int32 Code;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString ErrorDescription;

	UPROPERTY(BlueprintReadWrite, Category = TapTap)
	FString Message;
};

USTRUCT(BlueprintType)
struct FTapTapProfile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Avatar;

	UPROPERTY()
	FString OpenID;

	UPROPERTY()
	FString UnionID;
};

DECLARE_DYNAMIC_DELEGATE_FourParams(FTapTapLoginDelegate, ETapTapLogin::Type, Type, const FTapTapProfile&, Profile, const FTapTapAccessToken&, AccessToken, const FTapTapError&, Error);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FTapTapAntiAddictionNotifyDelegate, int32, Code, const FString&, Message);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FTapTapPayLimitDelegate, bool, bStatus, const FString&, Message);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FTapTapPayResultDelegate, bool, bSuccess, const FString&, Message);

UCLASS()
class TAPTAP_API UTapTapBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// TapTap初始化
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "TapTapInit"))
	static void Init();

	// TapTap登录
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "TapTapLogin"))
	static void Login(FTapTapLoginDelegate Delegate);

	// TapTap登出
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "TapTapLogout"))
	static void Logout();

	// TapTap实名认证登陆
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "StartupTapTapAntiAddiction"))
	static void StartupAntiAddiction(const FString& UserID, FTapTapAntiAddictionNotifyDelegate Delegate, bool bIsTapUser = false);

	// TapTap实名认证年龄范围
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "GetTapTapAntiAddictionAgeRange"))
	int32 GetAntiAddictionAgeRange() const;

	// 获取TapTap实名认证Token
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "GetTapTapAntiAddictionToken"))
	FString GetAntiAddictionToken() const;

	// 获取TapTap实名认证剩余游戏时间
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "GetTapTapAntiAddictionRemainingTime"))
	int32 GetAntiAddictionRemainingTime() const;

	// 检查TapTap实名认证支付限制
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "CheckTapTapAntiAddictionPayLimit"))
	void CheckAntiAddictionPayLimit(int32 Amount, FTapTapPayLimitDelegate Delegate);
	
	// 提交TapTap实名认证支付结果
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "SubmitTapTapAntiAddictionPayResult"))
	void SubmitAntiAddictionPayResult(int32 Amount, FTapTapPayResultDelegate Delegate);
	
	// 测试TapTap实名认证支付
	UFUNCTION(BlueprintCallable, Category = TapTap, meta = (DisplayName = "TestTapTapAntiAddiction"))
	void TestAntiAddiction();
};
