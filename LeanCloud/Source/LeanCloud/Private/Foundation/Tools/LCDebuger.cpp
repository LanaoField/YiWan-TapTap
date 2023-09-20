#include "LCDebuger.h"

FLeanCloudLogDelegate FLCDebuger::LogDelegate;

void FLCDebuger::LogError(const FString& Msg) {
	LogDelegate.ExecuteIfBound(ELCLogLevel::Error, Msg);
}

void FLCDebuger::LogWarning(const FString& Msg) {
	LogDelegate.ExecuteIfBound(ELCLogLevel::Warning, Msg);
}

void FLCDebuger::LogDebug(const FString& Msg) {
	LogDelegate.ExecuteIfBound(ELCLogLevel::Debug, Msg);
}

void FLCDebuger::LogVerbose(const FString& Msg) {
	LogDelegate.ExecuteIfBound(ELCLogLevel::Verbose, Msg);
}
