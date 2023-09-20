#include "LCHttpRequest.h"

#include "GenericPlatform/GenericPlatformHttp.h"

void FLCHttpRequest::SetUrl(const FString& InUrl) {
	Url = InUrl;
}

FString FLCHttpRequest::GetOriginUrl() const {
	return Url;
}

FString FLCHttpRequest::GetUrl() const {
	FString UrlEncodingParametersStr = UrlEncodingParameters(UrlParameters);
	if (UrlEncodingParametersStr.IsEmpty()) {
		return Url;
	} else {
		if (Url.Contains(TEXT("?"))) {
			return FString::Printf(TEXT("%s&%s"), *Url, *UrlEncodingParametersStr);
		} else {
			return FString::Printf(TEXT("%s?%s"), *Url, *UrlEncodingParametersStr);
		}
	}
}

TArray<TTuple<FString, FString>> LCQueryStringPairsFromKeyAndValue(const FString& Key, const FLCValue Value) {
	TArray<TTuple<FString, FString>> QueryStringComponents;
	
	if (Value.IsMapType()) {
		for (auto AsMap : Value.AsMap()) {
			if (!AsMap.Value.IsNoneType()) {
				FString SubKey;
				if (Key.IsEmpty()) {
					SubKey = AsMap.Key;
				} else {
					SubKey = FString::Printf(TEXT("%s[%s]"), *Key, *AsMap.Key);
				}
				QueryStringComponents.Append(LCQueryStringPairsFromKeyAndValue(SubKey, AsMap.Value));
			}
		}
	} else if (Value.IsArrayType()) {
		for (auto AsArray : Value.AsArray()) {
			QueryStringComponents.Append(LCQueryStringPairsFromKeyAndValue(Key + "[]", AsArray));
		}
	} else {
		QueryStringComponents.Add(TTuple<FString, FString>(Key, Value.AsString()));
	}

	return MoveTemp(QueryStringComponents);
}

FString FLCHttpRequest::UrlEncodingParameters(const TLCMap& Parameters) {
	TArray<TTuple<FString, FString>> Pairs = LCQueryStringPairsFromKeyAndValue("", Parameters);
	TArray<FString> KeyValues;
	for (auto Parameter : Pairs) {
		KeyValues.Add(FString::Printf(TEXT("%s=%s"), *FGenericPlatformHttp::UrlEncode(Parameter.Key), *FGenericPlatformHttp::UrlEncode(Parameter.Value)));
	}
	return FString::Join(KeyValues, TEXT("&"));
}

