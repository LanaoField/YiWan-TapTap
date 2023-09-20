#pragma once
#include "LCObject.h"

enum class ELCHttpMethod :uint8 {
	GET,
	POST,
	PUT,
	DELETE,
};

inline const TCHAR* LexToString(ELCHttpMethod HttpMethod) {
	switch (HttpMethod) {
	case ELCHttpMethod::GET: return TEXT("GET");
		break;
	case ELCHttpMethod::POST: return TEXT("POST");
		break;
	case ELCHttpMethod::PUT: return TEXT("PUT");
		break;
	case ELCHttpMethod::DELETE: return TEXT("DELETE");
		break;
	}
	return TEXT("");
}

class FLCHttpRequest {
public:
	ELCHttpMethod HttpMethod = ELCHttpMethod::GET;
	void SetUrl(const FString& InUrl);
	FString GetOriginUrl() const;
	FString GetUrl() const;

	TMap<FString, FString> Headers;
	TLCMap UrlParameters;
	TLCMap BodyParameters;
	TLCArray BodyArray;
	static FString UrlEncodingParameters(const TLCMap& Parameters);

private:
	FString Url;
};
