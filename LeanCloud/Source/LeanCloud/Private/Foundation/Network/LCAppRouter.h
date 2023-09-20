#pragma once
#include "LCApplication.h"

class FLCAppRouter {
public:
	static FString APIVersion;
	FLCAppRouter(const TSharedPtr<FLCApplication>& Ptr): ApplicationPtr(Ptr) {};
	FString GetRouteUrl(const FString& Path) const;

	FString GetFilePath(const FString& FileName) const;

	bool SaveFile(const FString& FileName, const TArray<uint8>& Data);
	bool LoadFile(const FString& FileName, TArray<uint8>& Data);
private:
	TWeakPtr<FLCApplication> ApplicationPtr;
};
