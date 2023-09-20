#pragma once
#include "LCObject.h"

class FLCUser;
// DECLARE_DELEGATE_TwoParams(FLeanCloudBoolResultDelegate, bool bIsSuccess, const FLCError& Error);
DECLARE_DELEGATE_TwoParams(FLeanCloudUserDelegate, TSharedPtr<FLCUser> UserPtr, const FLCError& Error);
DECLARE_DELEGATE_OneParam(FStringSignature, const FString& Signature);

class LEANCLOUDMOBILE_API FLCUser: public FLCObject {
public:

#if PLATFORM_IOS
	FLCUser(LCUser *InUser) : FLCObject((LCObject *)InUser) {};
	FLCUser(const FLCUser& OtherObject) : FLCObject(OtherObject) {}
	FLCUser(FLCUser&& OtherObject) : FLCObject(OtherObject) {}
#elif PLATFORM_ANDROID
	FLCUser(jobject InUser) : FLCObject(InUser) {};
	FLCUser(const LCJNI::Object& InUser) : FLCObject(InUser) {};
#endif

	void SetUsername(const FString& InUserName);
	void SetPassword(const FString& InPassword);
	void SetEmail(const FString& InEmail);
	void SetMobilePhoneNumber(const FString& InMobilePhoneNumber);
	FString GetUsername() const;
	FString GetPassword() const;
	FString GetSessionToken() const;
	FString GetEmail() const;
	FString GetMobilePhoneNumber() const;
	bool GetMobilePhoneVerified() const;
	bool GetEmailVerified() const;
	// 是否是匿名用户
	bool IsAnonymous() const;
	bool IsAuthenticated() const;

	// 当前用户
	static TSharedPtr<FLCUser> GetCurrentUser();

	// 退出登录
	static void LogOut();

	static void LoginAnonymously(FLeanCloudUserDelegate CallBack);

	static void LoginWithSessionToken(const FString& SessionToken, FLeanCloudUserDelegate CallBack);

	static void LoginWithAuthData(const TSharedPtr<FJsonObject>& AuthData, const FString& Platform, FLeanCloudUserDelegate CallBack);

	// 绑定/解绑第三方账户
	void AssociateWithAuthData(const TSharedPtr<FJsonObject>& AuthData, const FString& Platform, FLeanCloudBoolResultDelegate CallBack);
	void DisassociateWithPlatform(const FString& Platform, FLeanCloudBoolResultDelegate CallBack);

	// 客服获取token
	static void RetrieveShortToken(FStringSignature OnSuccess, FLCError::FDelegate OnFailed);

protected:

	// void OnRetrieveShortTokenCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FStringSignature OnSuccess, FLCError::FDelegate OnError);
	
	TSharedPtr<FJsonObject> GetAuthData() const;
};
