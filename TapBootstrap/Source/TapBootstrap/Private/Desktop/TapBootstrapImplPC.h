#pragma once
#include "TapBootstrapImpl.h"
#include "TUAccessToken.h"
#include "TULoginProfileModel.h"
#include "Interfaces/IHttpRequest.h"

class FTapBootstrapImplPC: public FTapBootstrapImpl{
public:
	virtual ~FTapBootstrapImplPC() override;
	virtual void Init(const FTUConfig& InConfig) override;
	virtual void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
		FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) override;

private:
	
	TSharedPtr<FTDSUser> CachedUser;
};


