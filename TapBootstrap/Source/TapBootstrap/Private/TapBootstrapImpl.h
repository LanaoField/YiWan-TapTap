#pragma once
#include "TDSLeaderBoardRanking.h"
#include "TDSUser.h"
#include "TUError.h"
#include "TUType.h"

class FTapBootstrapImpl {
public:
	static TSharedPtr<FTapBootstrapImpl> Get();
	virtual ~FTapBootstrapImpl();
	
	virtual void Init(const FTUConfig& InConfig);
	
	virtual void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError);

private:
	static TSharedPtr<FTapBootstrapImpl> Instance;
};
