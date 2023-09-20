#pragma once

struct FLCHelper {
	static FString GetSystemName();
	static void PerformOnGameThread(TFunction<void()> Function);
	static TArray<uint8> UTF8Encode(const FString& Content);
	static FString UTF8Encode(const TArray<uint8>& Data);
	static TArray<uint8> AesEncode(const TArray<uint8>& Content, const TArray<uint8>& Key, bool bIsPadding = true);
	static TArray<uint8> AesDecode(const TArray<uint8>& Content, const TArray<uint8>& Key, bool bIsPadding = true);
};