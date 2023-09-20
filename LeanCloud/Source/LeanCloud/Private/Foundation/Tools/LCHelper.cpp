#include "LCHelper.h"

#include "Async/Async.h"
#include "Misc/AES.h"

FString FLCHelper::GetSystemName() {
	FString OS;
#if PLATFORM_IOS
	OS = "iOS";
#elif PLATFORM_ANDROID
	OS = "Android";
#elif PLATFORM_MAC
	OS = "macOS";
#elif PLATFORM_WINDOWS
	OS = "Windows";
#elif PLATFORM_LINUX
	OS = "Linux";
#endif
	return OS;
}

void FLCHelper::PerformOnGameThread(TFunction<void()> Function) {
	if (!Function) {
		return;
	}
	if (IsInGameThread())
	{
		Function();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, Function);
	}
}

TArray<uint8> FLCHelper::UTF8Encode(const FString& content)
{
	FTCHARToUTF8 cnv(*content);
	int32 Len = cnv.Length();
	TArray<uint8> OutArray;
	OutArray.AddUninitialized(Len);
	FMemory::Memcpy(OutArray.GetData(), cnv.Get(), Len);
	return MoveTemp(OutArray);
}

FString FLCHelper::UTF8Encode(const TArray<uint8>& data)
{
	FUTF8ToTCHAR cnv((const ANSICHAR*)data.GetData(), data.Num());
	FString UTF8Text(cnv.Length(), cnv.Get());
	return MoveTemp(UTF8Text);
}

TArray<uint8> FLCHelper::AesEncode(const TArray<uint8>& content, const TArray<uint8>& key, bool isPadding)
{
	if (content.Num() == 0) {
		return content;
	}
	FAES::FAESKey AesKey;
	FMemory::Memcpy(AesKey.Key, key.GetData(), FMath::Min(FAES::FAESKey::KeySize, key.Num()));
	
	auto ContentSize = content.Num();
	auto ContentData = content;
	auto Alignment = FAES::AESBlockSize;

	uint32 Padding = Alignment - (ContentSize % Alignment);

	if (Padding == Alignment && isPadding == false)
	{
		Padding = 0;
	}

	ContentData.AddUninitialized(Padding);

	for (int32 FillIndex = ContentSize; FillIndex < ContentData.Num(); FillIndex++)
	{
		ContentData[FillIndex] = isPadding ? Padding : 0;
	}

	FAES::EncryptData(ContentData.GetData(), ContentData.Num(), AesKey);
	return MoveTemp(ContentData);
}

TArray<uint8> FLCHelper::AesDecode(const TArray<uint8>& content, const TArray<uint8>& key, bool isPadding)
{
	if (content.Num() == 0) {
		return content;
	}
	FAES::FAESKey AesKey;
	FMemory::Memcpy(AesKey.Key, key.GetData(), FMath::Min(FAES::FAESKey::KeySize, key.Num()));
	
	auto ContentData = content;
	auto Alignment = FAES::AESBlockSize;
	auto dirtyCount = content.Num() % Alignment;
	ContentData.SetNum(content.Num() - dirtyCount);

	FAES::DecryptData(ContentData.GetData(), ContentData.Num(), AesKey);
	if (ContentData.Num() == 0) {
		return ContentData;
	}

	if (isPadding)
	{
		auto Padding = ContentData.Last();
		if (ContentData.Num() >= Padding) {
			ContentData.SetNum(ContentData.Num() - Padding);
		}
	}
	return MoveTemp(ContentData);
	
}


