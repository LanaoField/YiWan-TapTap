#include "LCLeaderboard.h"

#include "Network/LCAppRouter.h"
#include "Network/LCHttpClient.h"
#include "Network/LCHttpRequest.h"
#include "Tools/LCHelper.h"

void FLCLeaderboard::SetApplicationPtr(const TSharedPtr<FLCApplication>& InPtr) {
	ApplicationPtr = InPtr;
}

TSharedPtr<FLCApplication> FLCLeaderboard::GetApplicationPtr() const {
	if (ApplicationPtr.IsValid()) {
		return ApplicationPtr.Pin();
	}
	else {
		return FLCApplication::DefaultPtr;
	}
}

FLCLeaderboard::FLCLeaderboard(const FString& Name, const Type& InMemberType) : StatisticName(Name),
	MemberType(InMemberType) {
	// if (!GetApplicationPtr().IsValid()) {
	// 	FLCError::Throw(ELCErrorCode::NoApplication);
	// }
}

void FLCLeaderboard::UpdateCurrentUserStatistics(const TMap<FString, double> Values,
                                                 const FStatisticsDelegate& OnSuccess,
                                                 const FLCError::FDelegate& OnError,
                                                 const TSharedPtr<FLCApplication>& AppPtr) {
	UpdateStatistics(nullptr, Values, OnSuccess, OnError, AppPtr);
}

void FLCLeaderboard::UpdateStatistics(const TSharedPtr<FLCUser>& User, const TMap<FString, double> Values,
                                      const FStatisticsDelegate& OnSuccess, const FLCError::FDelegate& OnError,
                                      const TSharedPtr<FLCApplication>& AppPtr) {
	TSharedPtr<FLCApplication> ApplicationsPtr = AppPtr;
	if (!ApplicationsPtr.IsValid()) {
		ApplicationsPtr = FLCApplication::DefaultPtr;
	}
	if (!ApplicationsPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnError.ExecuteIfBound(FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	TSharedPtr<FLCUser> TargetUser = User;
	if (!TargetUser.IsValid()) {
		TargetUser = ApplicationsPtr->GetCurrentUser();
	}
	if (!TargetUser.IsValid() || TargetUser->GetObjectId().IsEmpty()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnError.ExecuteIfBound(FLCError((int)ELCErrorCode::ObjectNotFound, "User is invaild"));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::POST;
	Request.SetUrl(ApplicationsPtr->AppRouter->GetRouteUrl(
		FString::Printf(TEXT("leaderboard/users/%s/statistics"), *TargetUser->GetObjectId())));
	for (auto Value : Values) {
		TLCMap Temp;
		Temp.Add("statisticName", Value.Key);
		Temp.Add("statisticValue", Value.Value);
		Request.BodyArray.Add(Temp);
	}
	ApplicationsPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                     [=](const FLCHttpResponse& InResponse) {
			                                     HandleStatisticsCallback(
				                                     OnSuccess, OnError, InResponse, ApplicationsPtr);
		                                     }));
}


void FLCLeaderboard::GetStatistics(const FString& TargetID, const FStatisticsDelegate& OnSuccess,
                                   const FLCError::FDelegate& OnError, TArray<FString> StatisticNames,
                                   const Type& InMemberType, const TSharedPtr<FLCApplication>& AppPtr) {
	TSharedPtr<FLCApplication> ApplicationsPtr = AppPtr;
	if (!ApplicationsPtr.IsValid()) {
		ApplicationsPtr = FLCApplication::DefaultPtr;
	}
	if (!ApplicationsPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnError.ExecuteIfBound(FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	FString Column = "";
	switch (InMemberType) {
	case User:
		Column = "users";
		break;
	case Entity:
		Column = "entities";
		break;
	case Object:
		Column = "objects";
		break;
	}
	if (StatisticNames.Num() > 0) {
		Request.UrlParameters.Add("statistics", FString::Join(StatisticNames, TEXT(",")));
	}
	Request.SetUrl(ApplicationsPtr->AppRouter->GetRouteUrl(
		FString::Printf(TEXT("leaderboard/%s/%s/statistics"), *Column, *TargetID)));
	ApplicationsPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                     [=](const FLCHttpResponse& InResponse) {
			                                     HandleStatisticsCallback(
				                                     OnSuccess, OnError, InResponse, ApplicationsPtr);
		                                     }));
}

void FLCLeaderboard::GetResults(const FRankingsDelegate& OnSuccess, const FLCError::FDelegate& OnError) {
	GetAroundResults("", OnSuccess, OnError);
}

void FLCLeaderboard::GetAroundResults(const FString& TargetID, const FRankingsDelegate& OnSuccess,
                                      const FLCError::FDelegate& OnError) {
	TSharedPtr<FLCApplication> ApplicationsPtr = GetApplicationPtr();
	if (!ApplicationsPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnError.ExecuteIfBound(FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	FString Column = "";
	switch (MemberType) {
	case User:
		Column = "user";
		break;
	case Entity:
		Column = "entity";
		break;
	case Object:
		Column = "object";
		break;
	}
	FString UrlPath = FString::Printf(TEXT("leaderboard/leaderboards/%s/%s/ranks"), *Column, *StatisticName);
	if (!TargetID.IsEmpty()) {
		UrlPath = UrlPath / TargetID;
	}
	if (Skip > 0) {
		Request.UrlParameters.Add("startPosition", Skip);
	}
	if (Limit > 0) {
		Request.UrlParameters.Add("maxResultsCount", Limit);
	}
	if (IncludeStatistics.Num() > 0) {
		Request.UrlParameters.Add("includeStatistics", FString::Join(IncludeStatistics, TEXT(",")));
	}
	if (SelectMemberKeys.Num() > 0) {
		Request.UrlParameters.Add("selectKeys", FString::Join(SelectMemberKeys, TEXT(",")));
	}
	if (Version > -1) {
		Request.UrlParameters.Add("version", Version);
	}
	if (WithCount) {
		Request.UrlParameters.Add("count", 1);
	}

	Request.SetUrl(ApplicationsPtr->AppRouter->GetRouteUrl(UrlPath));
	ApplicationsPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
		                                     [=](const FLCHttpResponse& InResponse) {
			                                     if (!InResponse.bIsSuccess()) {
				                                     FLCHelper::PerformOnGameThread([=]() {
					                                     OnError.ExecuteIfBound(InResponse.Error);
				                                     });
				                                     return;
			                                     }
			                                     if (!InResponse.Data.IsMapType()) {
				                                     FLCHelper::PerformOnGameThread([=]() {
					                                     OnError.ExecuteIfBound(FLCError(ELCErrorCode::MalformedData));
				                                     });
				                                     return;
			                                     }
			                                     // TArray<FLCLeaderboardStatistic> Statistics;
			                                     TArray<FLCLeaderboardRanking> Rankings;
			                                     TLCMap ResultMap = InResponse.Data.AsMap();
			                                     TLCArray ResultArr = InResponse.Data["results"].AsArray();
			                                     for (auto Item : ResultArr) {
				                                     FLCLeaderboardRanking Ranking(Item, ApplicationsPtr);
				                                     Rankings.Add(Ranking);
			                                     }
			                                     FLCHelper::PerformOnGameThread([=]() {
				                                     OnSuccess.ExecuteIfBound(
					                                     Rankings, InResponse.Data["count"].AsInteger());
			                                     });
		                                     }));
}

FString FLCLeaderboard::GetStatisticName() const {
	return StatisticName;
}

FLCLeaderboard::Type FLCLeaderboard::GetMemberType() const {
	return MemberType;
}

void FLCLeaderboard::HandleStatisticsCallback(const FStatisticsDelegate& OnSuccess, const FLCError::FDelegate& OnError,
                                              const FLCHttpResponse& InResponse,
                                              const TSharedPtr<FLCApplication>& AppPtr) {
	if (!InResponse.bIsSuccess()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnError.ExecuteIfBound(InResponse.Error);
		});
		return;
	}
	if (!InResponse.Data.IsMapType()) {
		FLCHelper::PerformOnGameThread([=]() {
			OnError.ExecuteIfBound(FLCError(ELCErrorCode::MalformedData));
		});
		return;
	}
	TArray<FLCLeaderboardStatistic> Statistics;
	TLCMap ResultMap = InResponse.Data.AsMap();
	TLCArray ResultArr = InResponse.Data["results"].AsArray();
	for (auto Item : ResultArr) {
		FLCLeaderboardStatistic Statistic(Item, AppPtr);
		Statistics.Add(Statistic);
	}
	FLCHelper::PerformOnGameThread([=]() {
		OnSuccess.ExecuteIfBound(Statistics);
	});
}
