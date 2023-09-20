#include "TapBootstrapImpliOS.h"
#include "TUError.h"
#include "TUType.h"
#include "IOSHelper.h"
#include "TapBootstrap.h"
#include "TUHelper.h"
#include "IOS/TapBootstrapObjcHelper.h"

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <TapBootstrapSDK/TapBootstrapSDK.h>

FTapBootstrapImpliOS::~FTapBootstrapImpliOS() {
}

void FTapBootstrapImpliOS::Init(const FTUConfig& InConfig) {
	FTUConfig::Init(InConfig);
	TapConfig* tapConfig = [TapConfig new];
	tapConfig.clientId = IOSHelper::Convert(InConfig.ClientID);
	if (InConfig.RegionType == ERegionType::CN) {
		tapConfig.region = TapSDKRegionTypeCN;
	} else {
		tapConfig.region = TapSDKRegionTypeIO;
	}
	tapConfig.clientToken = IOSHelper::Convert(InConfig.ClientToken);
	tapConfig.serverURL = IOSHelper::Convert(InConfig.ServerURL);
	if (InConfig.DBConfig.Enable) {
		TapDBConfig* dbConfig = [TapDBConfig new];
		dbConfig.channel = IOSHelper::Convert(InConfig.DBConfig.Channel);
		dbConfig.gameVersion = IOSHelper::Convert(InConfig.DBConfig.GameVersion);
		dbConfig.enable = InConfig.DBConfig.Enable;
		dbConfig.advertiserIDCollectionEnabled = InConfig.DBConfig.AdvertiserIDCollectionEnabled;
		tapConfig.dbConfig = dbConfig;
	}
	if (InConfig.BillboardConfig.IsValid())
	{
		NSMutableSet <NSArray *> *dimensionSet = [[NSMutableSet alloc] init];
		if (InConfig.BillboardConfig->Dimensions.Num() > 0)
		{
			for (TTuple<FString, FString>& T : InConfig.BillboardConfig->Dimensions)
			{
				[dimensionSet addObject:[NSArray arrayWithObjects:T.Key.GetNSString(), T.Value.GetNSString(), nil]];
			}
		}

		TapBillboardConfig *billboardCnConfig = [TapBillboardConfig new];
		billboardCnConfig.diemensionSet = dimensionSet; // 可选项
		billboardCnConfig.serverUrl = InConfig.BillboardConfig->BillboardUrl.GetNSString();
		
		tapConfig.tapBillboardConfig = billboardCnConfig;
	}
	[TapBootstrap initWithConfig:tapConfig];
}

void FTapBootstrapImpliOS::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) {

	[TDSLeaderBoardRanking queryTapFriendsLeaderBoardWithName:IOSHelper::Convert(Name) from:From limit:Limit callback:^(NSArray<TDSLeaderBoardRanking *> * _Nullable leaderboards, NSError * _Nullable error) {
		if (error) {
			FTUError Error = IOSHelper::Convert(error);
			TUHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error);
			});
		} else {
			TArray<FTDSLeaderBoardRanking> UE_Rankings;
			for (TDSLeaderBoardRanking *ranking in leaderboards) {
				FTDSLeaderBoardRanking UE_Ranking;
				UE_Ranking.Rank = ranking.rank;
				UE_Ranking.StatisticValue = ranking.statisticValue;
				UE_Ranking.StatisticName = IOSHelper::Convert(ranking.statisticName);
				UE_Ranking.User = TapBootstrapObjcHelper::Convert(ranking.user);
				UE_Rankings.Add(UE_Ranking);
			}
			TUHelper::PerformOnGameThread([=]() {
				OnSuccess.ExecuteIfBound(UE_Rankings);
			});
		}
	}];
}
