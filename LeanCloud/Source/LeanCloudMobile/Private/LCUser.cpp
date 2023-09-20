#include "LCUser.h"
#include "Tools/LCHelper.h"
#include "Tools/LCJsonHelper.h"
#if PLATFORM_IOS
#include "iOS/LCObjcHelper.h"
#elif PLATFORM_ANDROID
#include "Android/LCJNICallbackHelper.h"
#endif

static FString KeyUserName = "username";
static FString KeyPassword = "password";
static FString KeyEmail = "email";
static FString KeyMobilePhone = "mobilePhoneNumber";
static FString KeyMobilePhoneVerified = "mobilePhoneVerified";
static FString KeyEmailVerified = "emailVerified";
static FString KeyAuthData = "authData";
static FString KeyAuthDataAnonymous = "anonymous";
static FString KeySessionToken = "sessionToken";
static FString CLASS_NAME = "_User";


void FLCUser::SetUsername(const FString& InUserName) {
	SetStringForKey(KeyUserName, InUserName);
}

void FLCUser::SetPassword(const FString& InPassword) {
	SetStringForKey(KeyPassword, InPassword);
}

void FLCUser::SetEmail(const FString& InEmail) {
	SetStringForKey(KeyEmail, InEmail);
}
void FLCUser::SetMobilePhoneNumber(const FString& InMobilePhoneNumber) {
	SetStringForKey(KeyMobilePhone, InMobilePhoneNumber);
}
bool FLCUser::GetMobilePhoneVerified() const {
	return GetBooleanFromKey(KeyMobilePhoneVerified);
}
bool FLCUser::GetEmailVerified() const {
	return GetBooleanFromKey(KeyEmailVerified);
}

FString FLCUser::GetEmail() const {
	return GetStringFromKey(KeyEmail);
}

FString FLCUser::GetUsername() const {
	return GetStringFromKey(KeyUserName);
}

FString FLCUser::GetPassword() const {
	return GetStringFromKey(KeyPassword);
}

FString FLCUser::GetMobilePhoneNumber() const {
	return GetStringFromKey(KeyMobilePhone);
}

FString FLCUser::GetSessionToken() const {
	return GetStringFromKey(KeySessionToken);
}

bool FLCUser::IsAnonymous() const {
	auto AuthDataPtr = GetAuthData();
	if (!AuthDataPtr.IsValid()) {
		return false;
	}
	return AuthDataPtr->Values.Contains(KeyAuthDataAnonymous);
}

bool FLCUser::IsAuthenticated() const {
	return !GetSessionToken().IsEmpty();
}

TSharedPtr<FLCUser> FLCUser::GetCurrentUser() {
	static TSharedPtr<FLCUser> CurrentUserPtr = nullptr;
#if PLATFORM_IOS
	LCUser *currentUser = [LCUser currentUser];
	if (currentUser == nil) {
		CurrentUserPtr = nullptr;
		return nullptr;
	}
	if (!CurrentUserPtr.IsValid() || CurrentUserPtr->_Object != currentUser) {
		CurrentUserPtr = MakeShared<FLCUser>(currentUser);
	}
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	auto JaveUser = LCJNI.CallStaticObjectMethod(LeanCloudUEClass, "GetCurrentUser",
		"()Lcn/leancloud/LCUser;");
	if (!JaveUser) {
		CurrentUserPtr = nullptr;
		return nullptr;
	}
	if (!CurrentUserPtr.IsValid() || CurrentUserPtr->_Object != *JaveUser) {
		CurrentUserPtr = MakeShared<FLCUser>(JaveUser);
	}
#endif
	return CurrentUserPtr;
}

void FLCUser::LogOut() {
#if PLATFORM_IOS
	[LCUser logOut];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "LogOut", "()V");
#endif
}

void FLCUser::LoginAnonymously(FLeanCloudUserDelegate CallBack) {
#if PLATFORM_IOS
	[LCUser loginAnonymouslyWithCallback:^(LCUser * _Nullable user, NSError * _Nullable error) {
		auto UE_Error = LCObjcHelper::Convert(error);
		auto UE_User = LCObjcHelper::Convert(user);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(UE_User, UE_Error);
		});
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "LoginAnonymously",
	"(I)V", FLCJNICallbackHelper::AddCallBack(CallBack));
#endif
}

void FLCUser::LoginWithSessionToken(const FString& SessionToken, FLeanCloudUserDelegate CallBack) {
#if PLATFORM_IOS
	[LCUser becomeWithSessionTokenInBackground:LCObjcHelper::Convert(SessionToken) block:^(LCUser * _Nullable user, NSError * _Nullable error) {
		auto UE_Error = LCObjcHelper::Convert(error);
		auto UE_User = LCObjcHelper::Convert(user);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(UE_User, UE_Error);
		});
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "LoginWithSessionToken",
	"(Ljava/lang/String;I)V", *LCJNI.ToJavaString(SessionToken), FLCJNICallbackHelper::AddCallBack(CallBack));
#endif
}

void FLCUser::LoginWithAuthData(const TSharedPtr<FJsonObject>& AuthData, const FString& Platform, FLeanCloudUserDelegate CallBack) {
#if PLATFORM_IOS
	LCUser *user = [LCUser user];
	[user loginWithAuthData:LCObjcHelper::Convert(AuthData) platformId:LCObjcHelper::Convert(Platform) options:nil callback:^(BOOL succeeded, NSError * _Nullable error) {
		auto UE_Error = LCObjcHelper::Convert(error);
		TSharedPtr<FLCUser> UE_User = nullptr;
		if (succeeded) {
			UE_User = LCObjcHelper::Convert(user);
		}
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(UE_User, UE_Error);
		});
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	FString AuthDataStr = LCJsonHelper::GetJsonString(AuthData);
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "LoginWithAuthData",
	"(Ljava/lang/String;Ljava/lang/String;I)V", *LCJNI.ToJavaString(AuthDataStr), *LCJNI.ToJavaString(Platform), FLCJNICallbackHelper::AddCallBack(CallBack));
#endif
}

void FLCUser::AssociateWithAuthData(const TSharedPtr<FJsonObject>& AuthData, const FString& Platform, FLeanCloudBoolResultDelegate CallBack) {
#if PLATFORM_IOS
	[(LCUser *)_Object associateWithAuthData:LCObjcHelper::Convert(AuthData) platformId:LCObjcHelper::Convert(Platform) options:nil callback:^(BOOL succeeded, NSError * _Nullable error) {
		auto UE_Error = LCObjcHelper::Convert(error);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(succeeded, UE_Error);
		});
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	FString AuthDataStr = LCJsonHelper::GetJsonString(AuthData);
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "AssociateWithAuthData",
	"(Lcn/leancloud/LCUser;Ljava/lang/String;Ljava/lang/String;I)V",
	_Object, *LCJNI.ToJavaString(AuthDataStr), *LCJNI.ToJavaString(Platform), FLCJNICallbackHelper::AddCallBack(CallBack));
#endif
}

void FLCUser::DisassociateWithPlatform(const FString& Platform, FLeanCloudBoolResultDelegate CallBack) {
#if PLATFORM_IOS
	[(LCUser *)_Object disassociateWithPlatformId:LCObjcHelper::Convert(Platform) callback:^(BOOL succeeded, NSError * _Nullable error) {
		auto UE_Error = LCObjcHelper::Convert(error);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(succeeded, UE_Error);
		});
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "DisassociateWithPlatform",
	"(Lcn/leancloud/LCUser;Ljava/lang/String;I)V",
	_Object, *LCJNI.ToJavaString(Platform), FLCJNICallbackHelper::AddCallBack(CallBack));
#endif
}

void FLCUser::RetrieveShortToken(FStringSignature OnSuccess, FLCError::FDelegate OnFailed) {
#if PLATFORM_IOS
	[LCUser retrieveShortTokenWithCallback:^(NSString * _Nullable jwt, NSError * _Nullable error) {
		if (error) {
			auto UE_Error = LCObjcHelper::Convert(error);
			LCHelper::PerformOnGameThread([=]() {
				OnFailed.ExecuteIfBound(UE_Error);
			});
		} else {
			auto Token = LCObjcHelper::Convert(jwt);
			LCHelper::PerformOnGameThread([=]() {
				OnSuccess.ExecuteIfBound(Token);
			});
		}
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "RetrieveShortToken",
	"(I)V", FLCJNICallbackHelper::AddCallBack(OnSuccess, OnFailed));
#endif
}

TSharedPtr<FJsonObject> FLCUser::GetAuthData() const {
	const TSharedPtr<FJsonObject>* AuthDataJsonObject = nullptr;
	GetServerData()->TryGetObjectField(KeyAuthData, AuthDataJsonObject);
	return *AuthDataJsonObject;
}

#if PLATFORM_ANDROID
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void Java_com_lc_LeanCloudUE_onLeanCloudUserDelegate(JNIEnv* jenv, jclass thiz, jobject user, jstring ErrorReason, jint callBackID) {
		auto CallBackPtr = FLCJNICallbackHelper::FindCallBack<FLeanCloudUserDelegate>(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FLCError Error(-1, LCJNI.GetFStringFromParam(ErrorReason));
		TSharedPtr<FLCUser> UserPtr = nullptr;
		if (user != nullptr) {
			UserPtr = MakeShared<FLCUser>(user);
		}
		LCHelper::PerformOnGameThread([=]() {
			CallBackPtr->ExecuteIfBound(UserPtr, Error);
			FLCJNICallbackHelper::RemoveCallBack(callBackID);
		});
	}

	JNIEXPORT void Java_com_lc_LeanCloudUE_onFail(JNIEnv* jenv, jclass thiz, jstring ErrorReason, jint callBackID) {
		auto CallBackPtr = FLCJNICallbackHelper::FindCallBack<FLCError::FDelegate>(callBackID, 1);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FLCError Error(-1, LCJNI.GetFStringFromParam(ErrorReason));
		LCHelper::PerformOnGameThread([=]() {
			CallBackPtr->ExecuteIfBound(Error);
			FLCJNICallbackHelper::RemoveCallBack(callBackID);
		});
	}

	JNIEXPORT void Java_com_lc_LeanCloudUE_onRetrieveShortTokenSuccess(JNIEnv* jenv, jclass thiz, jstring content, jint callBackID) {
		auto CallBackPtr = FLCJNICallbackHelper::FindCallBack<FStringSignature>(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FString Content = LCJNI.GetFStringFromParam(content);
		LCHelper::PerformOnGameThread([=]() {
			CallBackPtr->ExecuteIfBound(Content);
			FLCJNICallbackHelper::RemoveCallBack(callBackID);
		});
	}

#ifdef __cplusplus
}
#endif
#endif



// void FLCUser::RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed)
// {
// 	if (TSharedPtr<FLCError> Error = FLCConfig::Get().CheckConfig())
// 	{
// 		OnFailed.ExecuteIfBound(*Error);
// 		return;
// 	}
// 	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
// 	HttpRequest->SetVerb(TEXT("GET"));
//
// 	FString Url = FLCConfig::Get().ServerURL + TEXT("/storage/1.1/users/tap-support/identity");
// 	HttpRequest->SetURL(Url);
//
// 	HttpRequest->SetHeader(TEXT("X-LC-Id"), FLCConfig::Get().ClientID);
// 	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetSyncString());
// 	HttpRequest->SetHeader(TEXT("X-LC-Session"), GetSessionToken());
// 	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//
// 	HttpRequest->OnProcessRequestComplete().BindSP(this, &FLCUser::OnRetrieveShortTokenCallback, OnSuccess, OnFailed);
//
// 	if (!HttpRequest->ProcessRequest())
// 	{
// 		OnFailed.ExecuteIfBound(FLCError(-1, TEXT("Net Error. http send failed.")));
// 	}
// }

// void FLCUser::OnRetrieveShortTokenCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FStringSignature OnSuccess, FLCError::FDelegate OnError)
// {
// 	if (bConnectedSuccessfully && Response && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
// 	{
// 		FString ContentString = Response->GetContentAsString();
// 		TSharedPtr<FJsonObject> JsonObject;
// 		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ContentString);
// 		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
// 		{
// 			FString Token;
// 			if (JsonObject->TryGetStringField(TEXT("identityToken"), Token))
// 			{
// 				OnSuccess.ExecuteIfBound(Token);
// 				return;
// 			}
// 		}
// 		OnError.ExecuteIfBound(FLCError(-1, FString::Printf(TEXT("Net Error, http response content:%s."), *ContentString)));
// 		return;
// 	}
// 	OnError.ExecuteIfBound(FLCError(-1, TEXT("Net Error, Http failed.")));
// }

