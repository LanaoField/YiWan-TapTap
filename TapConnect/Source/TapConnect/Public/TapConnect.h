#pragma once

class TAPCONNECT_API FTapConnect {
public:
	static void Init(const FString& ClientID, const FString& ClientToken,  bool bIsCN = true);
	static void SetEntryVisible(bool bVisible);
};