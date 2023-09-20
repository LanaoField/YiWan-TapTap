#include "LCQuery.h"

#include "Network/LCAppRouter.h"
#include "Network/LCHttpClient.h"
#include "Network/LCHttpRequest.h"
#include "Tools/LCDebuger.h"
#include "Tools/LCHelper.h"
#include "Tools/LCJsonHelper.h"

static FString EqualOp = TEXT("__eq");


FLCQuery::FLCQuery(const FString& InClassName) {
	ObjectClassName = InClassName;
}

FLCQuery::FLCQuery(const FString& InClassName, TSharedPtr<FLCApplication> InApplicationPtr) {
	ObjectClassName = InClassName;
	ApplicationPtr = InApplicationPtr;
}

void FLCQuery::Get(const FString& ObjectId, const FLeanCloudQueryObjectDelegate& CallBack) const {
	FLCQuery Query = FLCQuery(*this);
	Query.WhereEqualTo("objectId", ObjectId);
	Query.GetFirst(CallBack);
}

void FLCQuery::GetFirst(const FLeanCloudQueryObjectDelegate& CallBack) const {
	auto RequestParameters = GetParameters();
	RequestParameters.Add("limit") = 1;
	Find(RequestParameters, FLeanCloudQueryObjectsDelegate::CreateLambda([CallBack](TArray<TSharedPtr<FLCObject>> ObjectPtrs,
						   const FLCError& Error) {
		if (ObjectPtrs.Num() > 0) {
			CallBack.ExecuteIfBound(ObjectPtrs[0], Error);
		} else {
			if (Error.Code != 0) {
				CallBack.ExecuteIfBound(nullptr, Error);
			} else {
				CallBack.ExecuteIfBound(nullptr, FLCError(ELCErrorCode::ObjectNotFound));
			}
		}
	}));
}

void FLCQuery::Find(const FLeanCloudQueryObjectsDelegate& CallBack) const {
	Find(TLCMap(), CallBack);
}

void FLCQuery::Count(const FLeanCloudQueryCountDelegate& CallBack) const {
	TSharedPtr<FLCApplication> InApplicationPtr = GetApplicationPtr();
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(0, FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl(FLCHttpClient::GetEndpoint(ObjectClassName)));
	Request.UrlParameters = GetParameters();
	Request.UrlParameters.Add("count", 1);
	Request.UrlParameters.Add("limit", 0);
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
											 [=](const FLCHttpResponse& InResponse) {
												 if (InResponse.bIsSuccess()) {
													 FLCHelper::PerformOnGameThread([=]() {
														 CallBack.ExecuteIfBound(InResponse.GetCount(), InResponse.Error);
													 });
												 } else {
													 FLCHelper::PerformOnGameThread([=]() {
														 CallBack.ExecuteIfBound(0, InResponse.Error);
													 });
												 }
											 }));
}

TLCMap FLCQuery::GetLconWhere() const {
	TLCMap ResultMap;
	TLCArray AndArray;
	for (auto KeyMap : WhereMap) {
		TLCMap TempMap = TLCMap(KeyMap.Value);
		auto EqualValuePtr = TempMap.Find(EqualOp);
		if (EqualValuePtr) {
			TLCMap AndMap;
			AndMap.Add(KeyMap.Key, *EqualValuePtr);
			AndArray.Add(AndMap);
			TempMap.Remove(EqualOp);
		}
		if (TempMap.Num() > 0) {
			ResultMap.Add(KeyMap.Key, TempMap);
		}
	}
	for (auto ArrayMap : WhereArray) {
		if (ArrayMap.Key == "$and") {
			AndArray.Append(ArrayMap.Value);
		} else {
			ResultMap.Add(ArrayMap.Key, ArrayMap.Value);
		}
	}
	if (AndArray.Num() > 0) {
		ResultMap.Add("$and", AndArray);
	}
	return MoveTemp(ResultMap);
}

void FLCQuery::SetWhereString(const FString& InWhereStr) {
	WhereString = InWhereStr;
}

TSharedPtr<FLCApplication> FLCQuery::GetApplicationPtr() const {
	if (ApplicationPtr.IsValid()) {
		return ApplicationPtr.Pin();
	}
	else {
		return FLCApplication::DefaultPtr;
	}
}

FString FLCQuery::GetClassName() const {
	return ObjectClassName;
}

void FLCQuery::SetSelectKeys(const TArray<FString>& Keys) {
	SelectedKeys = TSet<FString>(Keys);
}

FLCQuery FLCQuery::And(const FLCQuery& Query) {
	TArray<FLCQuery> Querys;
	Querys.Add(*this);
	Querys.Add(Query);
	return And(Querys);
}

FLCQuery FLCQuery::Or(const FLCQuery& Query) {
	TArray<FLCQuery> Querys;
	Querys.Add(*this);
	Querys.Add(Query);
	return Or(Querys);
}

FLCQuery FLCQuery::And(const TArray<FLCQuery>& Querys) {
	return Combine("$and", Querys);
}

FLCQuery FLCQuery::Or(const TArray<FLCQuery>& Querys) {
	return Combine("$or", Querys);
}

FLCQuery& FLCQuery::WhereKeyIncluded(const FString& Key) {
	IncludedKeys.Add(Key);
	return *this;
}

FLCQuery& FLCQuery::WhereKeyExisted(const FString& Key) {
	AddWhereConstraint(Key, "$exists", true);
	return *this;
}

FLCQuery& FLCQuery::WhereKeyNotExisted(const FString& Key) {
	AddWhereConstraint(Key, "$exists", false);
	return *this;
}

FLCQuery& FLCQuery::WhereEqualTo(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, EqualOp, Value);
	return *this;
}

FLCQuery& FLCQuery::WhereNotEqualTo(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$ne", Value);
	return *this;
}

FLCQuery& FLCQuery::WhereLessThan(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$lt", Value);
	return *this;
}

FLCQuery& FLCQuery::WhereLessThanOrEqualTo(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$lte", Value);
	return *this;
}

FLCQuery& FLCQuery::WhereGreaterThan(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$gt", Value);
	return *this;
}

FLCQuery& FLCQuery::WhereGreaterThanOrEqualTo(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$gte", Value);
	return *this;
}

FLCQuery& FLCQuery::WhereContainedIn(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$in", Value.IsArrayType() ? Value : TLCArray({Value}));
	return *this;
}

FLCQuery& FLCQuery::WhereNotContainedIn(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$nin", Value.IsArrayType() ? Value : TLCArray({Value}));
	return *this;
}

FLCQuery& FLCQuery::WhereContainedAllIn(const FString& Key, const FLCValue& Value) {
	AddWhereConstraint(Key, "$all", Value.IsArrayType() ? Value : TLCArray({Value}));
	return *this;
}

FLCQuery& FLCQuery::WhereEqualToSize(const FString& Key, int64 Size) {
	AddWhereConstraint(Key, "$size", Size);
	return *this;
}

FLCQuery& FLCQuery::WhereNear(const FString& Key, const FLCGeoPoint& GeoPoint) {
	AddWhereConstraint(Key, "$nearSphere", GeoPoint);
	return *this;
}

FLCQuery& FLCQuery::WhereWithinGeoBox(const FString& Key, const FLCGeoPoint& Southwest, const FLCGeoPoint& Northeast) {
	TLCMap Box;
	Box.Add("$box", {Southwest, Northeast});
	AddWhereConstraint(Key, "$within", Box);
	return *this;
}

FLCQuery& FLCQuery::WhereWithinKilometers(const FString& Key, const FLCGeoPoint& GeoPoint, double MaxDistance,
	double MinDistance) {
	AddWhereConstraint(Key, "$nearSphere", GeoPoint);
	AddWhereConstraint(Key, "$maxDistanceInKilometers", MaxDistance);
	if (MinDistance > 0) {
		AddWhereConstraint(Key, "$minDistanceInKilometers", MinDistance);
	}
	return *this;
}

FLCQuery& FLCQuery::WhereWithinMiles(const FString& Key, const FLCGeoPoint& GeoPoint, double MaxDistance,
	double MinDistance) {
	AddWhereConstraint(Key, "$nearSphere", GeoPoint);
	AddWhereConstraint(Key, "$maxDistanceInMiles", MaxDistance);
	if (MinDistance > 0) {
		AddWhereConstraint(Key, "$minDistanceInMiles", MinDistance);
	}
	return *this;
}

FLCQuery& FLCQuery::WhereWithinRadians(const FString& Key, const FLCGeoPoint& GeoPoint, double MaxDistance,
	double MinDistance) {
	AddWhereConstraint(Key, "$nearSphere", GeoPoint);
	AddWhereConstraint(Key, "$maxDistanceInRadians", MaxDistance);
	if (MinDistance > 0) {
		AddWhereConstraint(Key, "$minDistanceInRadians", MinDistance);
	}
	return *this;
}

FLCQuery& FLCQuery::WhereMatchesKeyInQuery(const FString& Key, const FString& KeyInQuery, const FLCQuery& Query) {
	if (GetApplicationPtr() != Query.GetApplicationPtr()) {
		FLCDebuger::LogWarning("WhereMatchesKeyInQuery: ApplicationPtr is not same");
		return *this;
	}
	TLCMap SelectMap;
	SelectMap.Add("query", Query.GetLconValue());
	SelectMap.Add("key", KeyInQuery);
	AddWhereConstraint(Key, "$select", SelectMap);
	return *this;
}

FLCQuery& FLCQuery::WhereMatchesQuery(const FString& Key, const FLCQuery& Query) {
	if (GetApplicationPtr() != Query.GetApplicationPtr()) {
		FLCDebuger::LogWarning("WhereMatchesKeyInQuery: ApplicationPtr is not same");
		return *this;
	}
	AddWhereConstraint(Key, "$inQuery", Query.GetLconValue());
	return *this;
}

FLCQuery& FLCQuery::WhereDoesNotMatchKeyInQuery(const FString& Key, const FString& KeyInQuery, const FLCQuery& Query) {
	if (GetApplicationPtr() != Query.GetApplicationPtr()) {
		FLCDebuger::LogWarning("WhereMatchesKeyInQuery: ApplicationPtr is not same");
		return *this;
	}
	TLCMap SelectMap;
	SelectMap.Add("query", Query.GetLconValue());
	SelectMap.Add("key", KeyInQuery);
	AddWhereConstraint(Key, "$dontSelect", SelectMap);
	return *this;
}

FLCQuery& FLCQuery::WhereDoesNotMatchQuery(const FString& Key, const FLCQuery& Query) {
	if (GetApplicationPtr() != Query.GetApplicationPtr()) {
		FLCDebuger::LogWarning("WhereMatchesKeyInQuery: ApplicationPtr is not same");
		return *this;
	}
	AddWhereConstraint(Key, "$notInQuery", Query.GetLconValue());
	return *this;
}

FLCQuery& FLCQuery::WhereMatchedRegularExpression(const FString& Key, const FString& Expression, const FString& Option) {
	AddWhereConstraint(Key, "$regex", Expression);
	AddWhereConstraint(Key, "$options", Option);
	return *this;
}

FLCQuery& FLCQuery::WhereMatchedSubstring(const FString& Key, const FString& Substring) {
	AddWhereConstraint(Key, "$regex", Substring);
	return *this;
}

FLCQuery& FLCQuery::WherePrefixedBy(const FString& Key, const FString& Prefix) {
	AddWhereConstraint(Key, "$regex", "^" + Prefix);
	return *this;
}

FLCQuery& FLCQuery::WhereSuffixedBy(const FString& Key, const FString& Suffix) {
	AddWhereConstraint(Key, "$regex", Suffix + "$");
	return *this;
}

FLCQuery& FLCQuery::WhereRelatedTo(const FString& Key, const TSharedPtr<FLCObject>& Object) {
	AddWhereConstraint("$relatedTo", "object", Object);
	AddWhereConstraint("$relatedTo", "key", Key);
	return *this;
}

FLCQuery& FLCQuery::WhereOrderByAscending(const FString& Key) {
	OrderedKeys.Add(Key);
	return *this;
}

FLCQuery& FLCQuery::WhereOrderByDescending(const FString& Key) {
	OrderedKeys.Add("-" + Key);
	return *this;
}

void FLCQuery::Find(const TLCMap& Parameters, const FLeanCloudQueryObjectsDelegate& CallBack) const {
	auto InApplicationPtr = GetApplicationPtr();
	if (!InApplicationPtr.IsValid()) {
		FLCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(TArray<TSharedPtr<FLCObject>>(), FLCError(ELCErrorCode::NoApplication));
		});
		return;
	}
	TLCMap RequestParameters = Parameters;
	if (RequestParameters.Num() == 0) {
		RequestParameters = GetParameters();
	}
	FLCHttpRequest Request;
	Request.HttpMethod = ELCHttpMethod::GET;
	Request.SetUrl(InApplicationPtr->AppRouter->GetRouteUrl(FLCHttpClient::GetEndpoint(ObjectClassName)));
	Request.UrlParameters = RequestParameters;
	FString ResultClassName = ObjectClassName;
	InApplicationPtr->HttpClient->Request(Request, FLCHttpResponse::FDelegate::CreateLambda(
											 [=](const FLCHttpResponse& InResponse) {
												 if (InResponse.bIsSuccess()) {
												 	FString RealClassName = InResponse.Data["className"].AsString();
												    if (RealClassName.IsEmpty()) {
												    	RealClassName = ResultClassName;
												    }
													 TArray<TSharedPtr<FLCObject>> ObjectPtrs;
													 for (auto Result : InResponse.GetResults()) {
														 TSharedPtr<FLCObject> ObjectPtr = MakeShared<FLCObject>(
															 RealClassName);
														 ObjectPtr->ApplicationPtr = InApplicationPtr;
														 ObjectPtr->UpdateDataFromServer(Result.AsMap());
														 ObjectPtrs.Add(ObjectPtr);
													 }
												 	FLCHelper::PerformOnGameThread([=]() {
														 CallBack.ExecuteIfBound(ObjectPtrs, InResponse.Error);
													 });
												 } else {
													 FLCHelper::PerformOnGameThread([=]() {
														 CallBack.ExecuteIfBound(TArray<TSharedPtr<FLCObject>>(), InResponse.Error);
													 });
												 }
											 }));
}

TLCMap FLCQuery::GetParameters() const {
	auto Parameters = LconValueWithoutWhere();
	FString TempWhereString = LconWhereString();
	if (!TempWhereString.IsEmpty()) {
		Parameters.Add("where", TempWhereString);
	}
	return Parameters;
}

FString FLCQuery::LconWhereString() const {
	if (!WhereString.IsEmpty()) {
		return WhereString;
	}
	auto ConstraintDictionary = GetLconWhere();
	if (ConstraintDictionary.Num() > 0) {
		return FLCJsonHelper::GetJsonString(ConstraintDictionary);
	}
	return "";
}

FLCValue FLCQuery::GetLconValue() const
{
	auto Parameters = LconValueWithoutWhere();
	auto ConstraintDictionary = GetLconWhere();
	if (ConstraintDictionary.Num() > 0) {
		Parameters.Add("where", ConstraintDictionary);
	}
	return FLCValue(Parameters);
}

TLCMap FLCQuery::LconValueWithoutWhere() const {
	TLCMap Dictionary;
	Dictionary.Add("className", ObjectClassName);
	if (IncludedKeys.Num() > 0) {
		Dictionary.Add("include", FString::Join(IncludedKeys, TEXT(",")));
	}
	if (SelectedKeys.Num() > 0) {
		Dictionary.Add("keys", FString::Join(SelectedKeys, TEXT(",")));
	}
	if (OrderedKeys.Num() > 0) {
		Dictionary.Add("order", FString::Join(OrderedKeys, TEXT(",")));
	}
	if (Limit > 0) {
		Dictionary.Add("limit", Limit);
	}
	if (Skip > 0) {
		Dictionary.Add("skip", Skip);
	}
	if (IncludeACL) {
		Dictionary.Add("returnACL", "true");
	}
	for (auto ExtraParameter : ExtraParameters) {
		Dictionary.Add(ExtraParameter.Key, ExtraParameter.Value);
	}
	return MoveTemp(Dictionary);
}


FLCQuery FLCQuery::Combine(const FString& OpKey, const TArray<FLCQuery>& Querys) {
	if (Querys.Num() == 0) {
		FLCError::Throw(-1, "Querys is Empty");
	}
	if (Querys.Num() == 1) {
		return Querys[0];
	}
	auto AppPtr = Querys[0].GetApplicationPtr();
	auto ClassName = Querys[0].GetClassName();
	for (int i = 1; i < Querys.Num(); i++) {
		if (AppPtr != Querys[i].GetApplicationPtr()) {
			FLCError::Throw(-1, "Query's application is not same");
		}
		if (ClassName != Querys[i].GetClassName()) {
			FLCError::Throw(-1, "Query's class name is not same");
		}
	}
	FLCQuery ResultQuery(ClassName, AppPtr);
	for (auto Query : Querys) {
		ResultQuery.AddArrayConstraint(OpKey, Query.GetLconWhere());
	}
	return MoveTemp(ResultQuery);
}

void FLCQuery::AddArrayConstraint(const FString& OpKey, const FLCValue& Value) {
	TLCArray *OpArr = WhereArray.Find(OpKey);
	if (OpArr) {
		OpArr->Add(Value.GetLconValue());
	} else {
		WhereArray.Add(OpKey, TLCArray({Value.GetLconValue()}));
	}
}

void FLCQuery::AddWhereConstraint(const FString& Key, const FString& OpStr, const FLCValue& InValue) {
	TLCMap* OpMap = WhereMap.Find(Key);
	if (OpMap) {
		OpMap->Add(OpStr, InValue.GetLconValue());
	} else {
		TLCMap NewOpMap;
		NewOpMap.Add(OpStr, InValue.GetLconValue());
		WhereMap.Add(Key, NewOpMap);
	}
}
