#include "LCLeaderboardModel.h"

FLCLeaderboardStatistic::FLCLeaderboardStatistic(const FLCValue& Content, const TSharedPtr<FLCApplication>& AppPtr) {
	Name = Content["statisticName"].AsString();
	Version = Content["version"].AsInteger();
	Value = Content["statisticValue"].AsDouble();
	Entity = Content["entity"].AsString();
	Object = Content["object"].GetFromLconValue().AsObject();
	if (Object.IsValid()) {
		Object->SetApplicationPtr(AppPtr);
	}
	auto UserObject = Content["user"].GetFromLconValue().AsObject();
	if (UserObject.IsValid()) {
		User = UserObject->ConvertTo<FLCUser>();
		User->SetApplicationPtr(AppPtr);
	}
}

FLCLeaderboardRanking::FLCLeaderboardRanking(const FLCValue& Content, const TSharedPtr<FLCApplication>& AppPtr) {
	Rank = Content["rank"].AsInteger();
	EntityId = Content["entity"].AsString();
	Value = Content["statisticValue"].AsDouble();
	Object = Content["object"].GetFromLconValue().AsObject();
	if (Object.IsValid()) {
		Object->SetApplicationPtr(AppPtr);
	}
	auto UserObject = Content["user"].GetFromLconValue().AsObject();
	if (UserObject.IsValid()) {
		User = UserObject->ConvertTo<FLCUser>();
		User->SetApplicationPtr(AppPtr);
	}

	for (auto Item : Content["statistics"].AsArray()) {
		FLCLeaderboardStatistic Statistic(Item, AppPtr);
		Statistics.Add(Statistic);
	}
}
