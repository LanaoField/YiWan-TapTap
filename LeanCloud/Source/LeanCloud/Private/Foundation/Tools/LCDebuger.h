#pragma once
#include "LCApplication.h"

struct FLCDebuger {
	static FLeanCloudLogDelegate LogDelegate;
	
	static void LogError(const FString& Msg);
	static void LogWarning(const FString& Msg);
	static void LogDebug(const FString& Msg);
	static void LogVerbose(const FString& Msg);

};
