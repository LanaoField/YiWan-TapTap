#include "TapConnectImpl.h"
#include "TUDebuger.h"
#if PLATFORM_IOS
#include "iOS/TapConnectIOSImpl.h"
#elif PLATFORM_ANDROID
#include "Android/TapConnectAndroidImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

FTapConnectImpl::~FTapConnectImpl() {
}

TSharedPtr<FTapConnectImpl> FTapConnectImpl::Instance = nullptr;


TSharedPtr<FTapConnectImpl>& FTapConnectImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_IOS
		Instance = MakeShared<FTapConnectIOSImpl>();
#elif PLATFORM_ANDROID
		Instance = MakeShared<FTapConnectAndroidImpl>();
#else
		Instance = MakeShared<FTapConnectImpl>();
#endif
	}
	return Instance;
}

void FTapConnectImpl::Init(const FString& ClientID, const FString& ClientToken, bool bIsCN)
{
	UnsupportedPlatformsLog
}

void FTapConnectImpl::SetEntryVisible(bool bVisible) {
	UnsupportedPlatformsLog
}
