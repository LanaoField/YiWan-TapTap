#pragma once
#include "TapBootstrapImpl.h"

class FTapBootstrapImplAndroid: public FTapBootstrapImpl{
public:
	virtual ~FTapBootstrapImplAndroid() override;
	virtual void Init(const FTUConfig& InConfig) override;
	virtual void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
		FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) override;

private:
	int CallBackID = 0;
};
