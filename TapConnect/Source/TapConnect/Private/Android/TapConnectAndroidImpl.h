#pragma once
#include "TapConnectImpl.h"


class FTapConnectAndroidImpl: public FTapConnectImpl{
public:
	virtual ~FTapConnectAndroidImpl() override;
	virtual void Init(const FString& ClientID, const FString& ClientToken, bool bIsCN) override;
	virtual void SetEntryVisible(bool bVisible) override;
};