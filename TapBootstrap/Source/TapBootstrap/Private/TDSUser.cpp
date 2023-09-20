#include "TDSUser.h"
#include "JsonObjectConverter.h"
#include "TUJsonHelper.h"
#include "TapBootstrapImpl.h"
#include "TapUELogin.h"
#include "TUDeviceInfo.h"

#if PLATFORM_IOS
#include "IOSHelper.h"
#elif PLATFORM_ANDROID
#include "TapJNI.h"
#else

#endif


FString FTDSUser::KeyNickName = "nickname";
FString FTDSUser::KeyAvatar = "avatar";
FString FTDSUser::KeyShortID = "shortId";

TSharedPtr<FJsonObject> GetJsonObject(const TSharedPtr<FLCObject>& ObjectPtr) {
	if (!ObjectPtr.IsValid()) {
		return nullptr;
	}
#if PLATFORM_IOS || PLATFORM_ANDROID
	return ObjectPtr->GetServerData();
#else
	FString ServerDataStr = LC_MapToString(ObjectPtr->GetServerData());
	return TUJsonHelper::GetJsonObject(ServerDataStr);
#endif
}

TSharedPtr<FLCObject> MakeLCObject(const TSharedPtr<FJsonObject>& ObjectPtr) {
	if (!ObjectPtr.IsValid()) {
		return nullptr;
	}
#if PLATFORM_IOS
	LCObject *object = [LCObject object];
	[object objectFromDictionary:IOSHelper::Convert(ObjectPtr)];
	return MakeShared<FLCObject>(object);
#elif PLATFORM_ANDROID
	FString JsonStr = TUJsonHelper::GetJsonString(ObjectPtr);
	TapJNI::JNI JNI;
	auto ClassObject = JNI.FindClass("cn/leancloud/LCObject");
	auto object = JNI.CallStaticObjectMethod(ClassObject, "parseLCObject", "(Ljava/lang/String;)Lcn/leancloud/LCObject;", *JNI.ToJavaString(JsonStr));
	return MakeShared<FLCObject>(*object);
#else
	FString ClassName;
	if (!ObjectPtr->TryGetStringField("className", ClassName)) {
		return nullptr;
	}
	auto ServerData = LC_StringToMap(TUJsonHelper::GetJsonString(ObjectPtr));
	return MakeShared<FLCObject>(ClassName, ServerData);
#endif
}

TSharedPtr<FLCObject> MakeLCObject(const TSharedPtr<FJsonObject>& ObjectPtr, const FString& InClassName) {
	if (!ObjectPtr.IsValid()) {
		return nullptr;
	}
	TSharedPtr<FJsonObject> mObjectPtr = ObjectPtr;
	mObjectPtr->SetStringField("className", InClassName);
	return MakeLCObject(mObjectPtr);
}



FString FTDSUser::GetAvatar() const {
	return GetStringFromKey(KeyAvatar);
}

FString FTDSUser::GetNickName() const {
	return GetStringFromKey(KeyNickName);
}

FString FTDSUser::GetShortID() const {
	return GetStringFromKey(KeyShortID);
}

void FTDSUser::SetAvatar(const FString& InAvatar) {
	SetStringForKey(KeyAvatar, InAvatar);
}

void FTDSUser::SetNickName(const FString& InNickName) {
	SetStringForKey(KeyNickName, InNickName);
}

void FTDSUser::SetShortID(const FString& InShortID) {
	SetStringForKey(KeyShortID, InShortID);
}

#if !PLATFORM_IOS && !PLATFORM_ANDROID
void FTDSUser::SetStringForKey(const FString& InKey, const FString& InValue) {
	Set(InKey, InValue);
}

void FTDSUser::SetDoubleForKey(const FString& InKey, double InValue) {
	Set(InKey, InValue);
}

void FTDSUser::SetIntergerForKey(const FString& InKey, int64 InValue) {
	Set(InKey, InValue);
}

void FTDSUser::SetBooleanForKey(const FString& InKey, bool InValue) {
	Set(InKey, InValue);
}

FString FTDSUser::GetStringFromKey(const FString& InKey) const {
	return Get(InKey).AsString();
}

double FTDSUser::GetDoubleFromKey(const FString& InKey) const {
	return Get(InKey).AsDouble();
}

int64 FTDSUser::GetIntergerFromKey(const FString& InKey) const {
	return Get(InKey).AsInteger();
}

bool FTDSUser::GetBooleanFromKey(const FString& InKey) const {
	return Get(InKey).AsBoolean();
}

TSharedPtr<FJsonObject> FTDSUser::GetServerData() const {
	FString ServerDataStr = LC_MapToString(FLCUser::GetServerData());
	return TUJsonHelper::GetJsonObject(ServerDataStr);
}
#endif

void FTDSUser::Save(const FCallBackDelegate& CallBack) {
	TSharedPtr<FTDSUser> TempUser = nullptr;
	if (DoesSharedInstanceExist()) {
		TempUser = StaticCastSharedRef<FTDSUser>(AsShared());
	}
	else {
		TempUser = MakeShared<FTDSUser>(*this);
	}
	FLeanCloudBoolResultDelegate Delegate;
	LoginWithBoolCallBack(CallBack, Delegate, TempUser);
	TempUser->FLCUser::Save(Delegate);
}

TSharedPtr<FTDSUser> FTDSUser::GetCurrentUser() {
	return StaticCastSharedPtr<FTDSUser>(FLCUser::GetCurrentUser());
}

void FTDSUser::Logout() {
	FLCUser::LogOut();
	TapUELogin::Logout();
}

void FTDSUser::LoginAnonymously(const FCallBackDelegate& CallBack) {
	FLeanCloudUserDelegate Delegate;
	LoginWithUserCallBack(CallBack, Delegate);
#if PLATFORM_IOS || PLATFORM_ANDROID
	FLCUser::LoginAnonymously(Delegate);
#else
	FLCUser::LoginAnonymously(TUDeviceInfo::GetLoginId(), Delegate);
#endif
}

void FTDSUser::LoginWithTapTap(const TArray<FString>& Permissions, const FCallBackDelegate& CallBack) {
	auto TapLoginResult = [=](const TUAuthResult& Result) {
		switch (Result.GetType()) {
		case TUAuthResult::Success: {
			const TSharedPtr<FTUAccessToken> TapAccessToken = TapUELogin::GetAccessToken();
			const TSharedPtr<FTULoginProfileModel> TapProfile = TapUELogin::GetProfile();
			ensure(TapAccessToken && TapProfile);
			TSharedPtr<FJsonObject> AuthData = MakeShareable(new FJsonObject);
			AuthData->SetStringField(TEXT("access_token"), TapAccessToken->access_token);
			AuthData->SetStringField(TEXT("kid"), TapAccessToken->kid);
			AuthData->SetStringField(TEXT("mac_key"), TapAccessToken->mac_key);
			AuthData->SetStringField(TEXT("token_type"), TapAccessToken->token_type);
			AuthData->SetStringField(TEXT("mac_algorithm"), TapAccessToken->mac_algorithm);
			AuthData->SetStringField(TEXT("openid"), TapProfile->openid);
			AuthData->SetStringField(TEXT("unionid"), TapProfile->unionid);
			AuthData->SetStringField(TEXT("name"), TapProfile->name);
			AuthData->SetStringField(TEXT("avatar"), TapProfile->avatar);
			LoginWithAuthData("taptap", AuthData, CallBack);
		}
		break;
		case TUAuthResult::Cancel:
		case TUAuthResult::Fail:
			if (TSharedPtr<FTUError> Error = Result.GetError()) {
				CallBack.ExecuteIfBound(nullptr, *Error);
			}
			else {
				CallBack.ExecuteIfBound(nullptr, FTUError(-1, TEXT("TapTap login failed.")));
			}
			break;
		default:
			CallBack.ExecuteIfBound(nullptr, FTUError(-1, TEXT("Login taptap error")));
		}
	};
	TapUELogin::Login(Permissions, TapLoginResult);
}

void FTDSUser::BecomeWithSessionToken(const FString& SessionToken, const FCallBackDelegate& CallBack) {
	FLeanCloudUserDelegate Delegate;
	LoginWithUserCallBack(CallBack, Delegate);
	FLCUser::LoginWithSessionToken(SessionToken, Delegate);
}

void FTDSUser::LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData,
                                 const FCallBackDelegate& CallBack) {
	FLeanCloudUserDelegate Delegate;
	LoginWithUserCallBack(CallBack, Delegate);
#if PLATFORM_IOS || PLATFORM_ANDROID
	FLCUser::LoginWithAuthData(AuthData, Platform, Delegate);
#else
	FString AuthDataStr = TUJsonHelper::GetJsonString(AuthData);
	TLCMap AuthDataMap = LC_StringToMap(AuthDataStr);
	FLCUser::LoginWithAuthData(AuthDataMap, Platform, Delegate);
#endif
}

void FTDSUser::AssociateWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData,
                                     const FCallBackDelegate& CallBack) {
	TSharedPtr<FTDSUser> TempUser = nullptr;
	if (DoesSharedInstanceExist()) {
		TempUser = StaticCastSharedRef<FTDSUser>(AsShared());
	}
	else {
		TempUser = MakeShared<FTDSUser>(*this);
	}
	FLeanCloudBoolResultDelegate Delegate;
	LoginWithBoolCallBack(CallBack, Delegate, TempUser);
#if PLATFORM_IOS || PLATFORM_ANDROID
	TempUser->FLCUser::AssociateWithAuthData(AuthData, Platform, Delegate);
#else
	FString AuthDataStr = TUJsonHelper::GetJsonString(AuthData);
	TLCMap AuthDataMap = LC_StringToMap(AuthDataStr);
	TempUser->FLCUser::AssociateWithAuthData(AuthDataMap, Platform, Delegate);
#endif
}

void FTDSUser::DisassociateAuthData(const FString& Platform, const FCallBackDelegate& CallBack) {
	TSharedPtr<FTDSUser> TempUser = nullptr;
	if (DoesSharedInstanceExist()) {
		TempUser = StaticCastSharedRef<FTDSUser>(AsShared());
	}
	else {
		TempUser = MakeShared<FTDSUser>(*this);
	}
	FLeanCloudBoolResultDelegate Delegate;
	LoginWithBoolCallBack(CallBack, Delegate, TempUser);
	TempUser->FLCUser::DisassociateWithPlatform(Platform, Delegate);
}

void FTDSUser::RetrieveShortToken(const FStringSignature& OnSuccess, const FTUError::FDelegate& OnFailed) {
	FLCUser::RetrieveShortToken(OnSuccess, FLCError::FDelegate::CreateLambda([=](const FLCError& Error) {
		FTUError TapError;
		TapError.code = Error.Code;
		TapError.error_description = Error.Reason;
		OnFailed.ExecuteIfBound(TapError);
	}));
}


void FTDSUser::LoginWithUserCallBack(const FCallBackDelegate& CallBack, FLeanCloudUserDelegate& LCCallBack) {
	LCCallBack = FLeanCloudUserDelegate::CreateLambda([=](TSharedPtr<FLCUser> UserPtr, const FLCError& Error) {
		if (UserPtr.IsValid()) {
			CallBack.ExecuteIfBound(StaticCastSharedPtr<FTDSUser>(UserPtr), FTUError());
		}
		else {
			FTUError TapError;
			TapError.code = Error.Code;
			TapError.error_description = Error.Reason;
			CallBack.ExecuteIfBound(nullptr, TapError);
		}
	});
}

void FTDSUser::LoginWithBoolCallBack(const FCallBackDelegate& CallBack, FLeanCloudBoolResultDelegate& LCCallBack,
                                     const TSharedPtr<FTDSUser>& InUserPtr) {
	LCCallBack = FLeanCloudBoolResultDelegate::CreateLambda([=](bool bIsSuccess, const FLCError& Error) {
		if (bIsSuccess) {
			CallBack.ExecuteIfBound(InUserPtr, FTUError());
		}
		else {
			FTUError TapError;
			TapError.code = Error.Code;
			TapError.error_description = Error.Reason;
			CallBack.ExecuteIfBound(nullptr, TapError);
		}
	});
}
