#pragma once

class FTapConnectImpl {
public:
	virtual ~FTapConnectImpl();
	static TSharedPtr<FTapConnectImpl>& Get();
	
	virtual	void Init(const FString& ClientID, const FString& ClientToken, bool bIsCN = true);
	virtual	void SetEntryVisible(bool bVisible);

private:
	static TSharedPtr<FTapConnectImpl> Instance;
	
};
