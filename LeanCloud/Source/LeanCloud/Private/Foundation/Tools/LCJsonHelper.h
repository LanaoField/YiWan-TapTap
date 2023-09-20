#pragma once
#include "LCObject.h"
class FLCJsonHelper {
public:
	static FString GetJsonString(const TLCMap& Value, bool bNeedNull = false);
	static FString GetJsonString(const TLCArray& Value, bool bNeedNull = false);
	static FString GetJsonString(const TSharedPtr<FLCObject>& Value, bool bNeedNull = false);

	static FLCValue GetJsonValue(const FString& JsonStr);
};
