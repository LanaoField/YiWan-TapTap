#pragma once

class LEANCLOUDMOBILE_API FLCError {
public:
	DECLARE_DELEGATE_OneParam(FDelegate, const FLCError& Error);
	int64 Code;
	
	FString Reason;
	TSharedPtr<FJsonObject> UserInfo;
	FLCError() = default;
	FLCError(int64 InCode, const FString& InMsg): Code(InCode), Reason(InMsg) {};
	
};
