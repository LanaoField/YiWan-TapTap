#pragma once
#include "TapBootstrapImpl.h"

class FTapBootstrapImpliOS: public FTapBootstrapImpl{
public:
	virtual ~FTapBootstrapImpliOS() override;
	virtual void Init(const FTUConfig& InConfig) override;
	virtual void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
		FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) override;
};
