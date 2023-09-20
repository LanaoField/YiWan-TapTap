#include "TapBootstrapImpl.h"
#if PLATFORM_IOS
#include "iOS/TapBootstrapImpliOS.h"
typedef FTapBootstrapImpliOS TapBootstrapImplCommon;
#elif PLATFORM_ANDROID
#include "Android/TapBootstrapImplAndroid.h"
typedef FTapBootstrapImplAndroid TapBootstrapImplCommon;
#elif PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TapBootstrapImplPC.h"
typedef FTapBootstrapImplPC TapBootstrapImplCommon;
#else
typedef FTapBootstrapImpl TapBootstrapImplCommon;
#endif

#include "TUDebuger.h"

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("FTapBootstrapImpl Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<FTapBootstrapImpl> FTapBootstrapImpl::Instance;

TSharedPtr<FTapBootstrapImpl> FTapBootstrapImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShared<TapBootstrapImplCommon>();
	}
	return Instance;
}

FTapBootstrapImpl::~FTapBootstrapImpl() {
}

void FTapBootstrapImpl::Init(const FTUConfig& InConfig) {
	UnsupportedPlatformsLog
}


void FTapBootstrapImpl::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

