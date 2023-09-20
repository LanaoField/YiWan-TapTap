#include "TapBootstrapImplPC.h"
// #include "HttpModule.h"
// #include "LeanCloud.h"
#include "TapBootstrap.h"
#include "TapUELogin.h"
#include "TDSUser.h"
#include "TUHelper.h"
#include "TUJsonHelper.h"
// #include "Interfaces/IHttpResponse.h"
// #include "TUDeviceInfo.h"
#include "LCApplication.h"
#include "TUDeviceInfo.h"
#include "TUHttpManager.h"
#include "TUHttpRequest.h"


FTapBootstrapImplPC::~FTapBootstrapImplPC() {
}

void FTapBootstrapImplPC::Init(const FTUConfig& InConfig) {
	FTUConfig::Init(InConfig);
	FLCApplicationSettings Settings(InConfig.ClientID, InConfig.ClientToken, InConfig.ServerURL);
	FLCApplication::Register(Settings);

	// 初始化 TapLogin
	FTULoginConfig LoginConfig;
	LoginConfig.ClientID = InConfig.ClientID;
	LoginConfig.RegionType = InConfig.RegionType;
	TapUELogin::Init(LoginConfig);

	// 初始化 TapDB
	if (InConfig.DBConfig.Enable) {
		TUHelper::InvokeNoReturnFunction("TUDBReflection", "Init");
	}
}

void FTapBootstrapImplPC::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
                                                     FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess,
                                                     FTUError::FDelegate OnError) {
	TSharedPtr<FTDSUser> UserPtr = FTDSUser::GetCurrentUser();
	if (!UserPtr.IsValid()) {
		OnError.ExecuteIfBound(FTUError(80002, "please login first"));
		return;
	}
	const TSharedPtr<TUHttpRequest> request = MakeShared<TUHttpRequest>();
	request->Type = TUHttpRequest::Get;
	request->URL = FTUConfig::Get()->ServerURL / FString::Printf(
		TEXT("/friend/v2/taptap/leadboard?type=%s&skip=%d&limit=%d"), *Name, From, Limit);
	request->Headers.Add("X-LC-Session", UserPtr->GetSessionToken());
	request->Headers.Add("X-LC-Id", FTUConfig::Get()->ClientID);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
			if (response->state == TUHttpResponse::success) {
				TArray<FTDSLeaderBoardRanking> Rankings;
				FTUError Error;
				TSharedPtr<FJsonObject> JsonObject = TUJsonHelper::GetJsonObject(response->contentString);
				const TArray<TSharedPtr<FJsonValue>>* OutArray;
				if (JsonObject->TryGetArrayField("results", OutArray)) {
					for (auto JsonValue : *OutArray) {
						const TSharedPtr<FJsonObject>* ObjectP = nullptr;
						if (!JsonValue->TryGetObject(ObjectP)) {
							Error.code = -1;
							Error.msg = "invalid response json";
							break;
						}
						FTDSLeaderBoardRanking Ranking;
						(*ObjectP)->TryGetStringField("statisticName", Ranking.StatisticName);
						(*ObjectP)->TryGetNumberField("statisticValue", Ranking.StatisticValue);
						(*ObjectP)->TryGetNumberField("rank", Ranking.Rank);
						const TSharedPtr<FJsonObject>* UserObject;
						if ((*ObjectP)->TryGetObjectField("user", UserObject)) {
							Ranking.User = StaticCastSharedPtr<FTDSUser>(MakeLCObject(*UserObject, "_User"));
						}
						Rankings.Add(Ranking);
					}
				} else {
					Error.code = -1;
					Error.msg = "invalid response json";
				}

				if (Error.code == 0) {
					OnSuccess.ExecuteIfBound(Rankings);
				}
				else {
					OnError.ExecuteIfBound(Error);
				}
			}
			else {
				OnError.ExecuteIfBound(FTUError(response->httpCode, "request error"));
			}
		}
	);
	TUHttpManager::Get().request(request);
}
