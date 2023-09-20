#include "LCUser.h"
#include "Network/LCAppRouter.h"
#include "Network/LCHttpClient.h"
#include "Network/LCHttpRequest.h"
#include "Network/LCHttpResponse.h"
#include "Tools/LCHelper.h"
#include "Tools/LCJsonHelper.h"

FString LC_MapToString(const TLCMap& InMap) {
	return FLCJsonHelper::GetJsonString(InMap);
}

TLCMap LC_StringToMap(const FString& InString) {
	return FLCJsonHelper::GetJsonValue(InString).AsMap();
}

static FString KeyClassUserName = "_User";
static FString KeyUsername = "username";
static FString KeyPassword = "password";
static FString KeyEmail = "email";
static FString KeyMobilePhoneNumber = "mobilePhoneNumber";
static FString KeySessionToken = "sessionToken";
static FString KeyEmailVerified = "emailVerified";
static FString KeyMobilePhoneVerified = "mobilePhoneVerified";
static FString KeyAuthData = "authData";
static FString KeyAnonymous = "anonymous";

FLCUser::FLCUser() : FLCObject(KeyClassUserName) {
}

FLCUser::FLCUser(const FString& InObjectId) : FLCObject(KeyClassUserName, InObjectId) {
}

void FLCUser::SetUsername(const FString& InUserName) {
	Set(KeyUsername, InUserName);
}

void FLCUser::SetPassword(const FString& InPassword) {
	Set(KeyPassword, InPassword);
}

void FLCUser::SetEmail(const FString& InEmail) {
	Set(KeyEmail, InEmail);
}

void FLCUser::SetMobilePhoneNumber(const FString& InMobilePhoneNumber) {
	Set(KeyMobilePhoneNumber, InMobilePhoneNumber);
}

FString FLCUser::GetUsername() const {
	return Get(KeyUsername).AsString();
}

FString FLCUser::GetPassword() const {
	return Get(KeyPassword).AsString();
}

FString FLCUser::GetEmail() const {
	return Get(KeyEmail).AsString();
}

FString FLCUser::GetMobilePhoneNumber() const {
	return Get(KeyMobilePhoneNumber).AsString();
}

bool FLCUser::GetMobilePhoneVerified() const {
	return Get(KeyMobilePhoneVerified).AsBoolean();
}

bool FLCUser::GetEmailVerified() const {
	return Get(KeyEmailVerified).AsBoolean();
}

TSharedPtr<FLCUser> FLCUser::GetCurrentUser(const TSharedPtr<FLCApplication>& AppPtr) {
	TSharedPtr<FLCApplication> RealPtr = AppPtr;
	if (!RealPtr.IsValid()) {
		RealPtr = FLCApplication::DefaultPtr;
	}
	if (!RealPtr.IsValid()) {
		FLCError::Throw(ELCErrorCode::NoApplication);
	}
	return RealPtr->GetCurrentUser();
}

void FLCUser::SignUp(const FLeanCloudBoolResultDelegate& CallBack) {
	Save(CallBack);
}

void FLCUser::RequestLoginSmsCode(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack,
                                  const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	RequestBoolCallBack("requestLoginSmsCode", Paras, CallBack, AppPtr);
}

void FLCUser::SignUpOrLoginByMobilePhone(const FString& PhoneNumber, const FString& SmsCode,
                                         const FLeanCloudUserDelegate& CallBack,
                                         const TSharedPtr<FLCApplication>& AppPtr) {
	auto InApplicationPtr = GetRealAppPtr(AppPtr);
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(nullptr, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::POST;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl("usersByMobilePhone"));
	Request.BodyParameters.Add("smsCode", SmsCode);
	Request.BodyParameters.Add("mobilePhoneNumber", PhoneNumber);
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      PerformUserLoginDelegateByResponse(
				                                      CallBack, InResponse, InApplicationPtr);
		                                      }));
}

void FLCUser::Login(const FString& UserName, const FString& Password, const FLeanCloudUserDelegate& CallBack,
                    const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("username", UserName);
	Paras.Add("password", Password);
	Login(Paras, CallBack, AppPtr);
}

void FLCUser::LoginByEmail(const FString& Email, const FString& Password, const FLeanCloudUserDelegate& CallBack,
                           const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("email", Email);
	Paras.Add("password", Password);
	Login(Paras, CallBack, AppPtr);
}

void FLCUser::LoginByMobilePhone(const FString& PhoneNumber, const FString& Password,
                                 const FLeanCloudUserDelegate& CallBack,
                                 const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	Paras.Add("password", Password);
	Login(Paras, CallBack, AppPtr);
}

void FLCUser::LoginWithSessionToken(const FString& SessionToken, const FLeanCloudUserDelegate& CallBack,
                                    const TSharedPtr<FLCApplication>& AppPtr) {
	if (SessionToken.IsEmpty()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(nullptr, FLCError(ELCErrorCode::ParasInvaild));
		});
		return;
	}
	auto InApplicationPtr = GetRealAppPtr(AppPtr);
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(nullptr, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl("users/me"));
	Request.UrlParameters.Add("session_token", SessionToken);
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      PerformUserLoginDelegateByResponse(
				                                      CallBack, InResponse, InApplicationPtr);
		                                      }));
}

void FLCUser::LoginWithAuthData(const TLCMap& AuthData, const FString& Platform,
                                const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr) {
	auto InApplicationPtr = GetRealAppPtr(AppPtr);
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(nullptr, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::POST;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl("users"));
	TLCMap PlatformMap;
	PlatformMap.Add(Platform, AuthData);
	Request.BodyParameters.Add(KeyAuthData, PlatformMap);
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      PerformUserLoginDelegateByResponse(
				                                      CallBack, InResponse, InApplicationPtr);
		                                      }));
}

void FLCUser::LoginAnonymously(const FLeanCloudUserDelegate& CallBack, const TSharedPtr<FLCApplication>& AppPtr) {
	LoginAnonymously("", CallBack, AppPtr);
}

void FLCUser::LoginAnonymously(const FString& UUID, const FLeanCloudUserDelegate& CallBack,
	const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap AuthData;
	FString AnonymousID = UUID;
	if (AnonymousID.IsEmpty()) {
		AnonymousID = FGuid::NewGuid().ToString();
	}
	AuthData.Add("id", AnonymousID);
	LoginWithAuthData(AuthData, KeyAnonymous, CallBack, AppPtr);
}

void FLCUser::LogOut(const TSharedPtr<FLCApplication>& AppPtr) {
	TSharedPtr<FLCApplication> RealPtr = AppPtr;
	if (!RealPtr.IsValid()) {
		RealPtr = FLCApplication::DefaultPtr;
	}
	if (!RealPtr.IsValid()) {
		FLCError::Throw(ELCErrorCode::NoApplication);
	}
	return RealPtr->SetCurrentUser(nullptr);
}

void FLCUser::RequestEmailVerify(const FString& Email, const FLeanCloudBoolResultDelegate& CallBack,
                                 const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("email", Email);
	RequestBoolCallBack("requestEmailVerify", Paras, CallBack, AppPtr);
}

void FLCUser::RequestMobilePhoneVerify(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack,
                                       const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	RequestBoolCallBack("requestMobilePhoneVerify", Paras, CallBack, AppPtr);
}

void FLCUser::VerifyMobilePhone(const FString& PhoneNumber, const FString& SmsCode,
                                const FLeanCloudBoolResultDelegate& CallBack,
                                const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	RequestBoolCallBack("verifyMobilePhone/" + SmsCode, Paras, CallBack, AppPtr);
}

void FLCUser::RequestSMSCodeForUpdatingPhoneNumber(const FString& PhoneNumber,
                                                   const FLeanCloudBoolResultDelegate& CallBack,
                                                   const FLCSMSOption& Option) {
	TLCMap Paras = Option.GenerateParas();
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	RequestBoolCallBack("requestChangePhoneNumber", Paras, CallBack, Option.ApplicationPtr);
}

void FLCUser::VerifySMSCodeForUpdatingPhoneNumber(const FString& PhoneNumber, const FString& SmsCode,
                                                  const FLeanCloudBoolResultDelegate& CallBack,
                                                  const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	Paras.Add("code", SmsCode);
	RequestBoolCallBack("changePhoneNumber", Paras, CallBack, AppPtr);
}

void FLCUser::VerifySessionToken(const FString& SessionToken, const FLeanCloudBoolResultDelegate& CallBack,
                                 const TSharedPtr<FLCApplication>& AppPtr) {
	if (SessionToken.IsEmpty()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError(ELCErrorCode::ParasInvaild));
		});
		return;
	}
	auto InApplicationPtr = GetRealAppPtr(AppPtr);
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl("users/me"));
	Request.UrlParameters.Add("session_token", SessionToken);
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      InResponse.PerformBoolDelegateByResponse(CallBack);
		                                      }));
}

void FLCUser::RequestPasswordResetByEmail(const FString& Email, const FLeanCloudBoolResultDelegate& CallBack,
                                          const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("email", Email);
	RequestBoolCallBack("requestPasswordReset", Paras, CallBack, AppPtr);
}

void FLCUser::RequestPasswordResetBySmsCode(const FString& PhoneNumber, const FLeanCloudBoolResultDelegate& CallBack,
                                            const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("mobilePhoneNumber", PhoneNumber);
	RequestBoolCallBack("requestPasswordResetBySmsCode", Paras, CallBack, AppPtr);
}

void FLCUser::ResetPasswordBySmsCode(const FString& SmsCode, const FString& NewPassword,
                                     const FLeanCloudBoolResultDelegate& CallBack,
                                     const TSharedPtr<FLCApplication>& AppPtr) {
	TLCMap Paras;
	Paras.Add("password", NewPassword);
	RequestBoolCallBack("resetPasswordBySmsCode/" + SmsCode, Paras, CallBack, AppPtr, false);
}

void FLCUser::UpdatePassword(const FString& OldPassword, const FString& NewPassword,
                             const FLeanCloudBoolResultDelegate& CallBack) {
	TLCMap Paras;
	Paras.Add("old_password", OldPassword);
	Paras.Add("new_password", NewPassword);
	PutUserWithBoolCallBack("updatePassword", Paras, CallBack);
}

bool FLCUser::IsAuthenticated() const {
	return !GetSessionToken().IsEmpty();
}

void FLCUser::AssociateWithAuthData(const TLCMap& AuthData, const FString& Platform,
                                    const FLeanCloudBoolResultDelegate& CallBack) {
	TLCMap Paras;
	TLCMap PlatformMap;
	PlatformMap.Add(Platform, AuthData);
	Paras.Add(KeyAuthData, PlatformMap);
	PutUserWithBoolCallBack("", Paras, CallBack);
}

void FLCUser::DisassociateWithPlatform(const FString& Platform, const FLeanCloudBoolResultDelegate& CallBack) {
	TLCMap Paras;
	TLCMap OpMap;
	OpMap.Add("__op", "Delete");
	Paras.Add(FString::Printf(TEXT("%s.%s"), *KeyAuthData, *Platform), OpMap);
	PutUserWithBoolCallBack("", Paras, CallBack);
}

bool FLCUser::IsAnonymous() const {
	auto AuthDataPtr = GetServerData().Find(KeyAuthData);
	if (AuthDataPtr && AuthDataPtr->AsMap().Find(KeyAnonymous)) {
		return true;
	}
	return false;
}

void FLCUser::RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed, const TSharedPtr<FLCApplication>& AppPtr) {
	auto CurrentUser = GetCurrentUser(AppPtr);
	if (!CurrentUser.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnFailed.ExecuteIfBound(FLCError((int)ELCErrorCode::InvalidType, " Current User is Invalid"));
		});
		return;
	}
	if (CurrentUser->GetSessionToken().IsEmpty() || CurrentUser->GetObjectId().IsEmpty()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnFailed.ExecuteIfBound(FLCError((int)ELCErrorCode::InvalidType, "User is Invalid"));
		});
		return;
	}
	auto InApplicationPtr = CurrentUser->GetApplicationPtr();
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnFailed.ExecuteIfBound(FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	FString UrlPath = InApplicationPtr->GetServerUrl() / "storage" / FLCAppRouter::APIVersion /
		"users/tap-support/identity";
	Request.SetUrl(UrlPath);
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      FLCHelper::PerformOnGameThread([=]() {
				                                      if (InResponse.bIsSuccess()) {
					                                      FString IdentityToken = InResponse.Data["identityToken"].
						                                      AsString();
					                                      if (IdentityToken.IsEmpty()) {
						                                      OnFailed.ExecuteIfBound(
							                                      FLCError(ELCErrorCode::MalformedData));
					                                      }
					                                      else {
						                                      OnSuccess.ExecuteIfBound(IdentityToken);
					                                      }
				                                      }
				                                      else {
					                                      OnFailed.ExecuteIfBound(InResponse.Error);
				                                      }
			                                      });
		                                      }));
}

TSharedPtr<FLCApplication> FLCUser::GetRealAppPtr(const TSharedPtr<FLCApplication>& AppPtr) {
	if (AppPtr.IsValid()) {
		return AppPtr;
	}
	if (FLCApplication::DefaultPtr.IsValid()) {
		return FLCApplication::DefaultPtr;
	}
	else {
		return nullptr;
	}
}

void FLCUser::Login(const TLCMap& Parameters, const FLeanCloudUserDelegate& CallBack,
                    const TSharedPtr<FLCApplication>& AppPtr) {
	auto InApplicationPtr = GetRealAppPtr(AppPtr);
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(nullptr, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::POST;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl("login"));
	Request.BodyParameters = Parameters;
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      PerformUserLoginDelegateByResponse(
				                                      CallBack, InResponse, InApplicationPtr);
		                                      }));
}

void FLCUser::PerformUserLoginDelegateByResponse(const FLeanCloudUserDelegate& CallBack,
                                                 const FLCHttpResponse& InResponse,
                                                 const TSharedPtr<FLCApplication>& AppPtr) {
	if (InResponse.bIsSuccess()) {
		TSharedPtr<FLCUser> UserPtr = FLCObject::CreateObject<FLCUser>(
			InResponse.Data.AsMap(), AppPtr);
		if (UserPtr.IsValid()) {
			AppPtr->SetCurrentUser(UserPtr);
			FLCHelper::PerformOnGameThread([=]() {
				CallBack.ExecuteIfBound(UserPtr, InResponse.Error);
			});
		}
		else {
			FLCHelper::PerformOnGameThread([=]() {
				CallBack.ExecuteIfBound(
					nullptr, FLCError(ELCErrorCode::MalformedData));
			});
		}
	}
	else {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(nullptr, InResponse.Error);
		});
	}
}

void FLCUser::RequestBoolCallBack(const FString& Path, const TLCMap& InParas,
                                  const FLeanCloudBoolResultDelegate& CallBack,
                                  const TSharedPtr<FLCApplication>& AppPtr, bool bIsPost) {
	auto InApplicationPtr = GetRealAppPtr(AppPtr);
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = bIsPost ? ELCHttpMethod::POST : ELCHttpMethod::PUT;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl(Path));
	Request.BodyParameters = InParas;
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      InResponse.PerformBoolDelegateByResponse(CallBack);
		                                      }));
}

void FLCUser::PutUserWithBoolCallBack(const FString& Path, const TLCMap& InParas,
                                      const FLeanCloudBoolResultDelegate& CallBack) {
	if (GetSessionToken().IsEmpty() || GetObjectId().IsEmpty()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError((int)ELCErrorCode::InvalidType, "User is Invalid"));
		});
		return;
	}
	auto InApplicationPtr = GetApplicationPtr();
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(false, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	TSharedPtr<FLCUser> UserPtr;
	if (DoesSharedInstanceExist()) {
		UserPtr = StaticCastSharedRef<FLCUser>(AsShared());
	}
	else {
		UserPtr = MakeShared<FLCUser>(*this);
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::PUT;
	FString UrlPath = "users/" + GetObjectId();
	if (!Path.IsEmpty()) {
		UrlPath = UrlPath / Path;
	}
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl(UrlPath));
	Request.BodyParameters = InParas;
	Request.UrlParameters.Add("fetchWhenSave", true);
	Request.Headers.Add(FLCHttpClient::HeaderFieldName::Session, GetSessionToken());
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                      [=](const FLCHttpResponse& InResponse) {
			                                      if (InResponse.bIsSuccess()) {
				                                      UserPtr->UpdateDataFromServer(InResponse.Data.AsMap());
				                                      if (UserPtr == InApplicationPtr->GetCurrentUser()) {
					                                      InApplicationPtr->SetCurrentUser(UserPtr);
				                                      }
			                                      }
			                                      InResponse.PerformBoolDelegateByResponse(CallBack);
		                                      }));
}

FLCUser::~FLCUser() {
}

FString FLCUser::GetSessionToken() const {
	return Get(KeySessionToken).AsString();
}
