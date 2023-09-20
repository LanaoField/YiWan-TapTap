// Copyright 2022 CQUnreal. All Rights Reserved.

#include "TapTapBlueprintLibrary.h"
#include "TapTapModule.h"


void UTapTapBlueprintLibrary::Init()
{
	FTapTapModule::Get().Init();
}

void UTapTapBlueprintLibrary::Login(FTapTapLoginDelegate Delegate)
{
	FTapTapModule::Get().Login([Delegate](const TUAuthResult& Result) {

		FTapTapProfile Profile;

		TSharedPtr<FTULoginProfileModel> TULoginProfileModel = FTapTapModule::Get().GetProfile();
		if (TULoginProfileModel.IsValid())
		{
			Profile.Name = TULoginProfileModel->name;
			Profile.Avatar = TULoginProfileModel->avatar;
			Profile.OpenID = TULoginProfileModel->openid;
			Profile.UnionID = TULoginProfileModel->unionid;
		}

		FTapTapAccessToken AccessToken;

		TSharedPtr<FTUAccessToken> TUAccessToken = Result.GetToken();
		if (TUAccessToken.IsValid())
		{
			AccessToken.KID = TUAccessToken->kid;
			AccessToken.AccessToken = TUAccessToken->access_token;
			AccessToken.TokenType = TUAccessToken->token_type;
			AccessToken.MacKey = TUAccessToken->mac_key;
			AccessToken.MacAlgorithm = TUAccessToken->mac_algorithm;
			AccessToken.Scope = TUAccessToken->scope;
		}

		FTapTapError Error;

		TSharedPtr<FTUError> TUError = Result.GetError();
		if (TUError.IsValid())
		{
			Error.Code = TUError->code;
			Error.ErrorDescription = TUError->error_description;
			Error.Message = TUError->msg;
		}

		Delegate.ExecuteIfBound((ETapTapLogin::Type)Result.GetType(), Profile, AccessToken, Error);
	});
}

void UTapTapBlueprintLibrary::Logout()
{
	FTapTapModule::Get().Logout();
}

void UTapTapBlueprintLibrary::StartupAntiAddiction(const FString& UserID, FTapTapAntiAddictionNotifyDelegate Delegate, bool bIsTapUser)
{
	AntiAddictionUE::FCallBack CallBack = AntiAddictionUE::FCallBack::CreateLambda([Delegate](AntiAddictionUE::ResultHandlerCode Code, const FString& Message) {
		Delegate.ExecuteIfBound((int32)Code, Message);
	});
	FTapTapModule::Get().ListenAntiAddictionCallback(CallBack);
	FTapTapModule::Get().StartupAntiAddiction(UserID, bIsTapUser);
}

int32 UTapTapBlueprintLibrary::GetAntiAddictionAgeRange() const
{
	return FTapTapModule::Get().GetAntiAddictionAgeRange();
}

FString UTapTapBlueprintLibrary::GetAntiAddictionToken() const
{
	return FTapTapModule::Get().GetAntiAddictionToken();
}

int32 UTapTapBlueprintLibrary::GetAntiAddictionRemainingTime() const
{
	return FTapTapModule::Get().GetAntiAddictionRemainingTime();
}

void UTapTapBlueprintLibrary::CheckAntiAddictionPayLimit(int32 Amount, FTapTapPayLimitDelegate Delegate)
{
	FTapTapModule::Get().CheckAntiAddictionPayLimit(Amount, 
		[Delegate](bool bStatus) {
			Delegate.ExecuteIfBound(bStatus, TEXT(""));
		},
		[Delegate](const FString& Message) {
			Delegate.ExecuteIfBound(false, Message);
		});
}

void UTapTapBlueprintLibrary::SubmitAntiAddictionPayResult(int32 Amount, FTapTapPayResultDelegate Delegate)
{
	FTapTapModule::Get().SubmitAntiAddictionPayResult(Amount,
		[Delegate](bool bSuccess) {
			Delegate.ExecuteIfBound(bSuccess, TEXT(""));
		},
		[Delegate](const FString& Message) {
			Delegate.ExecuteIfBound(false, Message);
		});
}

void UTapTapBlueprintLibrary::TestAntiAddiction()
{
	FTapTapModule::Get().TestAntiAddiction();
}

