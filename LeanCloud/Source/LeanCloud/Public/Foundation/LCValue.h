#pragma once
// #include "LCObject.h"
#include "LCGeoPoint.h"
#include "Dom/JsonObject.h"

struct FLCValueBase;
struct FLCValue;
class FLCObject;
typedef TMap<FString, FLCValue> TLCMap;
typedef TArray<FLCValue> TLCArray;

LEANCLOUD_API FString LC_MapToString(const TLCMap& InMap);
LEANCLOUD_API TLCMap LC_StringToMap(const FString& InString);

template<typename T> struct TIsLCValueType           { enum { Value = false }; };
template<>           struct TIsLCValueType<FString> { enum { Value = true  }; };
template<>           struct TIsLCValueType<int> { enum { Value = true  }; };
template<>           struct TIsLCValueType<double> { enum { Value = true  }; };
template<>           struct TIsLCValueType<bool> { enum { Value = true  }; };
template<>           struct TIsLCValueType<TLCArray> { enum { Value = true  }; };
template<>           struct TIsLCValueType<TLCMap> { enum { Value = true  }; };
template<>           struct TIsLCValueType<FDateTime> { enum { Value = true  }; };
template<>           struct TIsLCValueType<FLCGeoPoint> { enum { Value = true  }; };
template<>           struct TIsLCValueType<TSharedPtr<FLCObject>> { enum { Value = true  }; };
template<>           struct TIsLCValueType<TArray<uint8>> { enum { Value = true  }; };
template<>           struct TIsLCValueType<FLCValue> { enum { Value = true  }; };

struct LEANCLOUD_API FLCValue {

	FLCValue();
	FLCValue(const FString& InValue);
	FLCValue(int InValue);
	FLCValue(int64 InValue);
	FLCValue(double InValue);
	FLCValue(bool InValue);
	FLCValue(const TLCArray& InValue);
	FLCValue(const TLCMap& InValue);
	FLCValue(const FDateTime& InValue);
	FLCValue(const FLCGeoPoint& InValue);
	FLCValue(const TSharedPtr<FLCObject>& InValue);
	FLCValue(const TArray<uint8>& InValue);

	template <typename CharType,
	          typename = typename TEnableIf<TIsCharType<CharType>::Value>::Type>
	FORCEINLINE FLCValue(const CharType* Src) {
		SetStringValue(FString(Src));
	}

	template <typename... LCValueType>
	FLCValue(const LCValueType&... Values) {
		TLCArray TempArray;
		AddArrayValue(TempArray, Values...);
	}
	
	FLCValue(FLCValue&&) = default;
	FLCValue(const FLCValue&) = default;
	FLCValue& operator=(FLCValue&&) = default;
	FLCValue& operator=(const FLCValue&) = default;

	bool operator==(const FLCValue& Rhs);
	bool operator!=(const FLCValue& Rhs);
	FORCEINLINE FLCValue operator[](const FString& Key) const
	{
		auto ResultPtr = AsMap().Find(Key);
		if (ResultPtr) {
			return *ResultPtr;
		} else {
			return FLCValue();
		}
	}

	FORCEINLINE FLCValue operator[](int Index) const
	{
		auto Array = AsArray();
		if (Index < 0 || Index >= Array.Num()) {
			return FLCValue();
		}
		return Array[Index];
	}

	FLCValue GetLconValue() const;
	FLCValue GetFromLconValue() const;

	void Reset();

	bool IsNoneType() const;
	bool IsStringType() const;
	bool IsDoubleType() const;
	bool IsIntegerType() const;
	bool IsNumberType() const;
	bool IsBooleanType() const;
	bool IsArrayType() const;
	bool IsMapType() const;
	bool IsDateType() const;
	bool IsGeoPointType() const;
	bool IsObjectType() const;
	bool IsDataType() const;

	FString AsString() const;
	int64 AsInteger() const;
	double AsDouble() const;
	bool AsBoolean() const;
	TLCArray& AsArray() const;
	TLCMap& AsMap() const;
	FDateTime AsDate() const;
	FLCGeoPoint AsGeoPoint() const;
	TSharedPtr<FLCObject> AsObject() const;
	TArray<uint8> AsData() const;
	friend LEANCLOUD_API FArchive& operator<<(FArchive& Ar, FLCValue& InValue);
	
	~FLCValue();

private:

	void SetStringValue(const FString& InValue);
	void SetArrayValue(const TLCArray& InValue);

	template <typename LCValueType,
		  typename = typename TEnableIf<TIsLCValueType<LCValueType>::Value>::Type>
	void AddArrayValue(TLCArray& Current, const LCValueType& InValue) {
		Current.Add(InValue);
		SetArrayValue(Current);
	}
	
	template <typename LCValueType, typename... OtherLCValueTypes>
	void AddArrayValue(TLCArray& Current, const LCValueType& InValue, const OtherLCValueTypes&... OtherValues) {
		Current.Add(FLCValue(InValue));
		AddArrayValue(Current, OtherValues...);
	}
	
	TSharedPtr<FLCValueBase> ValuePtr;
};



