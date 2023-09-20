#include "LCAppRouter.h"
#include "LCError.h"
#include "Foundation/Tools/LCDebuger.h"
#include "Misc/FileHelper.h"


FString FLCAppRouter::APIVersion = "1.1";

FString FLCAppRouter::GetRouteUrl(const FString& Path) const {
	return ApplicationPtr.Pin()->GetServerUrl() / APIVersion / Path;
}

FString FLCAppRouter::GetFilePath(const FString& FileName) const {
	if (!ApplicationPtr.IsValid()) {
		FLCError::Throw(ELCErrorCode::NoApplication);
	}
	return FPaths::ProjectSavedDir() / ApplicationPtr.Pin()->GetAppId() / FileName;
}

bool FLCAppRouter::SaveFile(const FString& FileName, const TArray<uint8>& Data) {
	FString Path = GetFilePath(FileName);
	FLCDebuger::LogVerbose("Save File: " + FileName);
	return FFileHelper::SaveArrayToFile(Data, *Path);
}

bool FLCAppRouter::LoadFile(const FString& FileName, TArray<uint8>& Data) {
	FString Path = GetFilePath(FileName);
	FLCDebuger::LogVerbose("Load File: " + FileName);
	return FFileHelper::LoadFileToArray(Data, *Path);
}
