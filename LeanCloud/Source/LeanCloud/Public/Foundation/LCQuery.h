#pragma once
#include "LCApplication.h"
#include "LCGeoPoint.h"
#include "LCError.h"
#include "LCValue.h"

class FLCObject;
DECLARE_DELEGATE_TwoParams(FLeanCloudQueryObjectDelegate, TSharedPtr<FLCObject> ObjectPtr, const FLCError& Error);
DECLARE_DELEGATE_TwoParams(FLeanCloudQueryObjectsDelegate, TArray<TSharedPtr<FLCObject>> ObjectPtrs,
                           const FLCError& Error);
DECLARE_DELEGATE_TwoParams(FLeanCloudQueryCountDelegate, int Count, const FLCError& Error);


class LEANCLOUD_API FLCQuery {
public:
	FLCQuery() = default;
	FLCQuery(const FString& InClassName);
	FLCQuery(const FString& InClassName, TSharedPtr<FLCApplication> InApplicationPtr);

	void Get(const FString& ObjectId, const FLeanCloudQueryObjectDelegate& CallBack) const;
	void GetFirst(const FLeanCloudQueryObjectDelegate& CallBack) const;
	void Find(const FLeanCloudQueryObjectsDelegate& CallBack) const;
	void Count(const FLeanCloudQueryCountDelegate& CallBack) const;

	TLCMap GetLconWhere() const;
	FLCValue GetLconValue() const;

	int64 Limit = 0;
	int64 Skip = 0;
	bool IncludeACL = false;
	void SetWhereString(const FString& InWhereStr);

	TSharedPtr<FLCApplication> GetApplicationPtr() const;
	FString GetClassName() const;

	void SetSelectKeys(const TArray<FString>& Keys);

	FLCQuery And(const FLCQuery& Query);
	FLCQuery Or(const FLCQuery& Query);
	static FLCQuery And(const TArray<FLCQuery>& Querys);
	static FLCQuery Or(const TArray<FLCQuery>& Querys);
	
	/* Key */
	FLCQuery& WhereKeyIncluded(const FString& Key);
	FLCQuery& WhereKeyExisted(const FString& Key);
	FLCQuery& WhereKeyNotExisted(const FString& Key);

	/* Equality */
	FLCQuery& WhereEqualTo(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereNotEqualTo(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereLessThan(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereLessThanOrEqualTo(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereGreaterThan(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereGreaterThanOrEqualTo(const FString& Key, const FLCValue& Value);

	/* Array */
	FLCQuery& WhereContainedIn(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereNotContainedIn(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereContainedAllIn(const FString& Key, const FLCValue& Value);
	FLCQuery& WhereEqualToSize(const FString& Key, int64 Size);

	/* GeoPoint */
	FLCQuery& WhereNear(const FString& Key, const FLCGeoPoint& GeoPoint);
	FLCQuery& WhereWithinGeoBox(const FString& Key, const FLCGeoPoint& Southwest, const FLCGeoPoint& Northeast);
	FLCQuery& WhereWithinKilometers(const FString& Key, const FLCGeoPoint& GeoPoint, double MaxDistance,
	                                double MinDistance = 0);
	FLCQuery& WhereWithinMiles(const FString& Key, const FLCGeoPoint& GeoPoint, double MaxDistance,
	                           double MinDistance = 0);
	FLCQuery& WhereWithinRadians(const FString& Key, const FLCGeoPoint& GeoPoint, double MaxDistance,
	                             double MinDistance = 0);

	/* Query */
	FLCQuery& WhereMatchesKeyInQuery(const FString& Key, const FString& KeyInQuery, const FLCQuery& Query);
	FLCQuery& WhereMatchesQuery(const FString& Key, const FLCQuery& Query);
	FLCQuery& WhereDoesNotMatchKeyInQuery(const FString& Key, const FString& KeyInQuery, const FLCQuery& Query);
	FLCQuery& WhereDoesNotMatchQuery(const FString& Key, const FLCQuery& Query);

	/* String */
	FLCQuery& WhereMatchedRegularExpression(const FString& Key, const FString& Expression, const FString& Option = "");
	FLCQuery& WhereMatchedSubstring(const FString& Key, const FString& Substring);
	FLCQuery& WherePrefixedBy(const FString& Key, const FString& Prefix);
	FLCQuery& WhereSuffixedBy(const FString& Key, const FString& Suffix);

	/* Relation */
	FLCQuery& WhereRelatedTo(const FString& Key, const TSharedPtr<FLCObject>& Object);

	/* Order */
	FLCQuery& WhereOrderByAscending(const FString& Key);
	FLCQuery& WhereOrderByDescending(const FString& Key);
	
private:
	
	FString ObjectClassName;
	TWeakPtr<FLCApplication> ApplicationPtr;
	TSet<FString> IncludedKeys;
	TSet<FString> SelectedKeys;
	TArray<FString> OrderedKeys;
	TMap<FString, TLCMap> WhereMap;
	TMap<FString, TLCArray> WhereArray;
	TLCMap ExtraParameters;
	FString WhereString;

	
	void Find(const TLCMap& Parameters, const FLeanCloudQueryObjectsDelegate& CallBack) const;
	TLCMap GetParameters() const;
	FString LconWhereString() const;
	TLCMap LconValueWithoutWhere() const;
	static FLCQuery Combine(const FString& OpKey, const TArray<FLCQuery>& Querys);
	void AddArrayConstraint(const FString& OpKey, const FLCValue& Value);
	void AddWhereConstraint(const FString& Key, const FString& OpStr, const FLCValue& InValue);

};
