#pragma once
#include "LCObject.h"
#include "LCSMS.h"

class FLCUser;
class FLCHttpResponse;

DECLARE_DELEGATE_TwoParams(FLeanCloudUserDelegate, TSharedPtr<FLCUser> UserPtr, const FLCError& Error);
DECLARE_DELEGATE_OneParam(FStringSignature, const FString& Signature);

class LEANCLOUD_API FLCUser : public FLCObject {
public:
	FLCUser();
	FLCUser(const FString& InObjectId);
	virtual ~FLCUser() override;
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

	// emailVerified

	// 当前用户
	static TSharedPtr<FLCUser> GetCurrentUser(const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	// 登录
	void SignUp(const FLeanCloudBoolResultDelegate& CallBack = nullptr);
	static void RequestLoginSmsCode(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void SignUpOrLoginByMobilePhone(const FString& PhoneNumber, const FString& SmsCode, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void Login(const FString& UserName, const FString& Password, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void LoginByEmail(const FString& Email, const FString& Password, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void LoginByMobilePhone(const FString& PhoneNumber, const FString& Password, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void LoginWithSessionToken(const FString& SessionToken, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void LoginWithAuthData(const TLCMap& AuthData, const FString& Platform, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void LoginAnonymously(const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void LoginAnonymously(const FString& UUID, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	// 退出登录
	static void LogOut(const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	// 邮箱及手机验证
	static void RequestEmailVerify(const FString& Email, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void RequestMobilePhoneVerify(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void VerifyMobilePhone(const FString& PhoneNumber, const FString& SmsCode, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	// 绑定、修改手机号
	static void RequestSMSCodeForUpdatingPhoneNumber(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack, const FLCSMSOption& Option = FLCSMSOption());
	static void VerifySMSCodeForUpdatingPhoneNumber(const FString& PhoneNumber, const FString& SmsCode, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);

	// 验证SessionToken是否有效
	static void VerifySessionToken(const FString& SessionToken, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	bool IsAuthenticated() const;

	// 重置密码
	static void RequestPasswordResetByEmail(const FString& Email, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void RequestPasswordResetBySmsCode(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	static void ResetPasswordBySmsCode(const FString& SmsCode, const FString& NewPassword, const FLeanCloudBoolResultDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr = nullptr);
	void UpdatePassword(const FString& OldPassword, const FString& NewPassword, const FLeanCloudBoolResultDelegate& CallBack);
	

	// 绑定/解绑第三方账户
	void AssociateWithAuthData(const TLCMap& AuthData, const FString& Platform, const FLeanCloudBoolResultDelegate& CallBack);
	void DisassociateWithPlatform(const FString& Platform, const FLeanCloudBoolResultDelegate& CallBack);

	// 是否是匿名用户
	bool IsAnonymous() const;

	// 客服获取token
	static void RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed, const TSharedPtr<FLCApplication>& AppPtr = nullptr);


private:
	
	static TSharedPtr<FLCApplication> GetRealAppPtr(const TSharedPtr<FLCApplication>& AppPtr);
	static void Login(const TLCMap& Parameters, const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr);
	static void PerformUserLoginDelegateByResponse(const FLeanCloudUserDelegate& CallBack, const FLCHttpResponse& InResponse, const TSharedPtr<FLCApplication>& AppPtr);

	static void RequestBoolCallBack(const FString& Path, const TLCMap& InParas, const FLeanCloudBoolResultDelegate& CallBack,
	const TSharedPtr<FLCApplication>& AppPtr, bool bIsPost = true);

	void PutUserWithBoolCallBack(const FString& Path, const TLCMap& InParas, const FLeanCloudBoolResultDelegate& CallBack);
};
