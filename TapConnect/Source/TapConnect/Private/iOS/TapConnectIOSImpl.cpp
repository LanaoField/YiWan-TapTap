#include "iOS/TapConnectIOSImpl.h"
#include "IOSHelper.h"

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <TapConnectSDK/TapConnectSDK.h>

FTapConnectIOSImpl::~FTapConnectIOSImpl() {
}

void FTapConnectIOSImpl::Init(const FString& ClientID, const FString& ClientToken, bool bIsCN)
{
	[TapConnect initWithClientId:IOSHelper::Convert(ClientID) clientToken:IOSHelper::Convert(ClientToken) isCN:bIsCN];
}

void FTapConnectIOSImpl::SetEntryVisible(bool bVisible) {
	[TapConnect setEntryVisible:bVisible];
}
