#pragma once
#include "LCObject.h"
#include "Network/LCHttpRequest.h"
#include "Network/LCHttpResponse.h"

class FLCHttpResponse;
class FLCObjectUpdater {
public:
	static void Save(const TArray<TSharedPtr<FLCObject>>& Objects, const FLeanCloudBoolResultDelegate& CallBack);
	static void Save(const TArray<TSharedPtr<FLCObject>>& Objects, const FLCSaveOption& Option, const FLeanCloudBoolResultDelegate& CallBack);
	static void Fetch(const TArray<TSharedPtr<FLCObject>>& Objects, const TArray<FString>& Keys, const FLeanCloudBoolResultDelegate& CallBack);
	static void Delete(const TArray<TSharedPtr<FLCObject>>& Objects, const FLeanCloudBoolResultDelegate& CallBack);
	

private:
	enum class EObjectVisitState {
		Unvisited,
		Visiting,
		Visited
	};
	static FLeanCloudBoolResultDelegate PerformCallBackOnGameThread(const FLeanCloudBoolResultDelegate& CallBack);

	static void Classify(const TArray<TSharedPtr<FLCObject>>& Objects, TArray<TSharedPtr<FLCObject>>& NewbornObjects, TArray<TSharedPtr<FLCObject>>& FamilyObjects);
	static void Visit(TMap<TSharedPtr<FLCObject>, EObjectVisitState>& VisitRecord, const FLCValue& Value, const FLCValue& Parent, const TFunction<void(const TSharedPtr<FLCObject>& UnvisitedObject, const FLCValue& TopValue)>& UnvisitedBlock);
	static void Save(const TArray<TSharedPtr<FLCObject>>& Objects, const TLCMap& InParas, const FLeanCloudBoolResultDelegate& CallBack);
	static TSharedPtr<FLCApplication> GetApplicationsPtr(const TArray<TSharedPtr<FLCObject>>& Objects);
	static void SaveInOneBatchRequest(const TArray<TSharedPtr<FLCObject>>& Objects, const TLCMap& InParas, const FLeanCloudBoolResultDelegate& CallBack);
	static FString GetBatchRequestPath(const FString& Path);
	static TLCMap GenerateBatchRequest(ELCHttpMethod InHttpMethod, const TLCMap& InParas, const TSharedPtr<FLCObject>& Object);
	static TLCMap GenerateBatchRequestBody(const TSharedPtr<FLCObject>& Object);
	static TSharedPtr<FLCError> HandleObjectFetchedResult(const TLCMap& Result, const TArray<TSharedPtr<FLCObject>>& Objects);
	static TSharedPtr<FLCError> HandleObjectFetchedResponse(const FLCHttpResponse& InResponse, const TArray<TSharedPtr<FLCObject>>& Objects);

};
