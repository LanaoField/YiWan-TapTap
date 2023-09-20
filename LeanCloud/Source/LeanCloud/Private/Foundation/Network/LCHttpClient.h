#pragma once
#include "LCApplication.h"
#include "LCHttpRequest.h"
#include "LCHttpResponse.h"

class FLCHttpClient {
public:
	struct HeaderFieldName {
		static FString Id;
		static FString Signature;
		static FString Session;
		static FString Production;
		static FString UserAgent;
		static FString Accept;
	};

	FLCHttpClient(const TSharedPtr<FLCApplication>& Ptr): ApplicationPtr(Ptr) {};
	void Request(const FLCHttpRequest& LCRequest, FLCHttpResponse::FDelegate CallBack);

	static FString GetEndpoint(const FString& ClassName);
private:
	TWeakPtr<FLCApplication> ApplicationPtr;

	FString CreateRequestSignature();

	TMap<FString, FString> CreateCommonHeaders(const FLCHttpRequest& LCRequest);
};
