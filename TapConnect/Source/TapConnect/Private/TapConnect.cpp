#include "TapConnect.h"

#include "TapConnectImpl.h"


void FTapConnect::Init(const FString& ClientID, const FString& ClientToken, bool bIsCN)
{
	FTapConnectImpl::Get()->Init(ClientID, ClientToken, bIsCN);
}

void FTapConnect::SetEntryVisible(bool bVisible) {
	FTapConnectImpl::Get()->SetEntryVisible(bVisible);
}
