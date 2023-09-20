#pragma once
#include "TapConnectImpl.h"

class FTapConnectIOSImpl: public FTapConnectImpl{
public:
	virtual ~FTapConnectIOSImpl() override;
	virtual void Init(const FString& ClientID, const FString& ClientToken, bool bIsCN) override;
	virtual void SetEntryVisible(bool bVisible) override;
};
