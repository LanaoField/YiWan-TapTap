#pragma once
#include "LCObject.h"
#include "LCValue.h"
#include "LCGeoPoint.h"

enum class ELCValueType {
	None,
	String,
	Integer,
	Double,
	Boolean,
	Array,
	Map,
	Date,
	GeoPoint,
	Data,
	Object
	// null
	// acl
};
// struct FLCValue;
struct FLCValueBase {

	virtual FString AsString() { return ""; }
	virtual int64 AsInteger() { return 0; }
	virtual double AsDouble() { return 0; }
	virtual bool AsBoolean() { return false; }
	virtual TLCArray& AsArray();
	virtual TLCMap& AsMap();
	virtual FDateTime AsDate() { return FDateTime(); }
	virtual FLCGeoPoint AsGeoPoint() { return FLCGeoPoint(); }
	virtual TSharedPtr<FLCObject> AsObject() {return nullptr;}
	virtual TArray<uint8> AsData() { return {}; }

	virtual FLCValue GetLconValue() = 0;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs)
	{
		return ValueType == Rhs->ValueType;
	}

	ELCValueType ValueType;
protected:
	FLCValueBase() : ValueType(ELCValueType::None) {
	};
	virtual ~FLCValueBase() = default;
	virtual FString GetType() const = 0;
};

struct FLCValueNone : FLCValueBase {
	FLCValueNone() {
		ValueType = ELCValueType::None;
	}

	virtual FLCValue GetLconValue() override;
	
	virtual FString GetType() const override { return TEXT("None"); };
};

struct FLCValueString : FLCValueBase {
	FLCValueString(const FString& InValue) : Value(InValue) {
		ValueType = ELCValueType::String;
	}

	virtual FLCValue GetLconValue() override;

	virtual FString AsString() override;
	virtual int64 AsInteger() override;
	virtual double AsDouble() override;
	virtual bool AsBoolean() override;
	virtual FDateTime AsDate() override;
	virtual TArray<uint8> AsData() override;

	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;


	FString Value;
	virtual FString GetType() const override { return TEXT("String"); };
};

struct FLCValueInteger : FLCValueBase {
	FLCValueInteger(int64 InValue) : Value(InValue) {
		ValueType = ELCValueType::Integer;
	}

	virtual FLCValue GetLconValue() override;

	virtual FString AsString() override;
	virtual int64 AsInteger() override;
	virtual double AsDouble() override;
	virtual bool AsBoolean() override;

	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	int64 Value;
	virtual FString GetType() const override { return TEXT("Integer"); };
};

struct FLCValueDouble : FLCValueBase {
	FLCValueDouble(double InValue) : Value(InValue) {
		ValueType = ELCValueType::Double;
	}

	virtual FLCValue GetLconValue() override;

	virtual FString AsString() override;
	virtual int64 AsInteger() override;
	virtual double AsDouble() override;
	virtual bool AsBoolean() override;

	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	double Value;
	virtual FString GetType() const override { return TEXT("Double"); };
};

struct FLCValueBoolean : FLCValueBase {
	FLCValueBoolean(bool InValue) : Value(InValue) {
		ValueType = ELCValueType::Boolean;
	}

	virtual FLCValue GetLconValue() override;

	virtual FString AsString() override;
	virtual int64 AsInteger() override;
	virtual double AsDouble() override;
	virtual bool AsBoolean() override;

	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	bool Value;
	virtual FString GetType() const override { return TEXT("Boolean"); };
};

struct FLCValueArray : FLCValueBase {
	FLCValueArray(const TLCArray& InValue) : Value(InValue) {
		ValueType = ELCValueType::Array;
	}

	virtual FLCValue GetLconValue() override;
	virtual TLCArray& AsArray() override;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	TLCArray Value;
	virtual FString GetType() const override { return TEXT("Array"); };
};

struct FLCValueMap : FLCValueBase {
	FLCValueMap(const TLCMap& InValue) : Value(InValue) {
		ValueType = ELCValueType::Map;
	}
	virtual FLCValue GetLconValue() override;
	virtual TLCMap& AsMap() override;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	TLCMap Value;
	virtual FString GetType() const override { return TEXT("Map"); };
};

struct FLCValueObject : FLCValueBase {
	FLCValueObject(const TSharedPtr<FLCObject>& InValue) : Value(InValue) {
		ValueType = ELCValueType::Object;
	}
	virtual FLCValue GetLconValue() override;
	virtual TSharedPtr<FLCObject> AsObject() override;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	TSharedPtr<FLCObject> Value;
	virtual FString GetType() const override { return TEXT("Object"); };
};

struct FLCValueDate : FLCValueBase {
	FLCValueDate(const FDateTime& InValue) : Value(InValue) {
		ValueType = ELCValueType::Date;
	}
	virtual FLCValue GetLconValue() override;
	virtual FDateTime AsDate() override;
	virtual FString AsString() override;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	FDateTime Value;
	virtual FString GetType() const override { return TEXT("Date"); };
};

struct FLCValueGeoPoint : FLCValueBase {
	FLCValueGeoPoint(const FLCGeoPoint& InValue) : Value(InValue) {
		ValueType = ELCValueType::GeoPoint;
	}
	virtual FLCValue GetLconValue() override;
	virtual FLCGeoPoint AsGeoPoint() override;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	FLCGeoPoint Value;
	virtual FString GetType() const override { return TEXT("GeoPoint"); };
};

struct FLCValueData : FLCValueBase {
	FLCValueData(const TArray<uint8>& InValue) : Value(InValue) {
		ValueType = ELCValueType::Data;
	}
	virtual FLCValue GetLconValue() override;
	virtual TArray<uint8> AsData() override;
	virtual FString AsString() override;
	virtual bool operator==(const TSharedPtr<FLCValueBase>& Rhs) override;

	TArray<uint8> Value;
	virtual FString GetType() const override { return TEXT("Data"); };
};
