#pragma once
#include "LCObject.h"

// DECLARE_DELEGATE_OneParam(FLeanCloudProgressDelegate, int64 Progress);
//
// class LEANCLOUD_API FLCFile : public FLCObject {
// public:
// 	FLCFile();
// 	FLCFile(const FString& Url, const FString& FileName);
// 	FLCFile(const TArray<uint8>& Data, const FString& FileName);
// 	virtual ~FLCFile() override;
//
// 	void SetMimeType(const FString& InMimeType);
// 	FString GetUrl() const;
//
// 	void Save(FLeanCloudProgressDelegate ProgressDelegate, FLeanCloudBoolResultDelegate CallBack);
//
// 	TSharedPtr<FJsonObject> MetaData;
//
// 	FString GetThumbnailUrl(bool ScaleToFit, int Width, int Height, int Quality = 100, const FString& Fmt = "png");
// };
