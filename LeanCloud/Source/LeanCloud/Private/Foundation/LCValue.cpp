#include "LCValue.h"
#include "LCValueBase.h"
#include "LCObject.h"

FLCValue::FLCValue() {
	ValuePtr = MakeShared<FLCValueNone>();
}

FLCValue::FLCValue(const FString& InValue) {
	ValuePtr = MakeShared<FLCValueString>(InValue);
}

FLCValue::FLCValue(int InValue) {
	ValuePtr = MakeShared<FLCValueInteger>(InValue);
}

FLCValue::FLCValue(int64 InValue) {
	ValuePtr = MakeShared<FLCValueInteger>(InValue);
}

FLCValue::FLCValue(double InValue) {
	ValuePtr = MakeShared<FLCValueDouble>(InValue);
}

FLCValue::FLCValue(bool InValue) {
	ValuePtr = MakeShared<FLCValueBoolean>(InValue);
}

FLCValue::FLCValue(const TLCArray& InValue) {
	ValuePtr = MakeShared<FLCValueArray>(InValue);
}

FLCValue::FLCValue(const TLCMap& InValue) {
	ValuePtr = MakeShared<FLCValueMap>(InValue);
}

FLCValue::FLCValue(const FDateTime& InValue) {
	ValuePtr = MakeShared<FLCValueDate>(InValue);
}

FLCValue::FLCValue(const FLCGeoPoint& InValue) {
	ValuePtr = MakeShared<FLCValueGeoPoint>(InValue);
}

FLCValue::FLCValue(const TSharedPtr<FLCObject>& InValue) {
	ValuePtr = MakeShared<FLCValueObject>(InValue);
}

FLCValue::FLCValue(const TArray<uint8>& InValue) {
	ValuePtr = MakeShared<FLCValueData>(InValue);
}

bool FLCValue::operator==(const FLCValue& Rhs)
{
	return *ValuePtr.Get() == Rhs.ValuePtr;
}

bool FLCValue::operator!=(const FLCValue& Rhs)
{
	return !(*this == Rhs);
}

FLCValue FLCValue::GetLconValue() const
{
	return ValuePtr->GetLconValue();
}

FLCValue FLCValue::GetFromLconValue() const {
	if (IsArrayType()) {
		TLCArray Array;
		for (auto AsArray : this->AsArray()) {
			Array.Add(AsArray.GetFromLconValue());
		}
		return Array;
	}
	else if (IsMapType()) {
		TLCMap Map = this->AsMap();
		FLCValue* TypeStrPtr = Map.Find("__type");
		if (TypeStrPtr && TypeStrPtr->IsStringType()) {
			FString TypeStr = TypeStrPtr->AsString();
			if (TypeStr == "Pointer") {
				TSharedPtr<FLCObject> Object = MakeShared<FLCObject>((*this)["className"].AsString(),
				                                                     (*this)["objectId"].AsString());
				return Object;
			}
			else if (TypeStr == "Date") {
				return (*this)["iso"].AsDate();
			}
			else if (TypeStr == "Bytes") {
				return (*this)["base64"].AsData();
			}
			else if (TypeStr == "GeoPoint") {
				return FLCGeoPoint((*this)["latitude"].AsDouble(), (*this)["longitude"].AsDouble());
			}
		}
		TLCMap ResultMap;
		for (auto Result : Map) {
			ResultMap.Add(Result.Key, Result.Value.GetFromLconValue());
		}
		return ResultMap;
	}
	else {
		return *this;
	}
}

void FLCValue::Reset() {
	ValuePtr = MakeShared<FLCValueNone>();
}

bool FLCValue::IsNoneType() const {
	return ValuePtr->ValueType == ELCValueType::None;
}

bool FLCValue::IsStringType() const {
	return ValuePtr->ValueType == ELCValueType::String;
}

bool FLCValue::IsDoubleType() const {
	return ValuePtr->ValueType == ELCValueType::Double;
}

bool FLCValue::IsIntegerType() const {
	return ValuePtr->ValueType == ELCValueType::Integer;
}

bool FLCValue::IsNumberType() const {
	return IsIntegerType() || IsDoubleType();
}

bool FLCValue::IsBooleanType() const {
	return ValuePtr->ValueType == ELCValueType::Boolean;
}

bool FLCValue::IsArrayType() const {
	return ValuePtr->ValueType == ELCValueType::Array;
}

bool FLCValue::IsMapType() const {
	return ValuePtr->ValueType == ELCValueType::Map;
}

bool FLCValue::IsDateType() const {
	return ValuePtr->ValueType == ELCValueType::Date;
}

bool FLCValue::IsGeoPointType() const {
	return ValuePtr->ValueType == ELCValueType::GeoPoint;

}

bool FLCValue::IsObjectType() const {
	return ValuePtr->ValueType == ELCValueType::Object;
}

bool FLCValue::IsDataType() const {
	return ValuePtr->ValueType == ELCValueType::Data;
}

FString FLCValue::AsString() const {
	return ValuePtr->AsString();
}

int64 FLCValue::AsInteger() const {
	return ValuePtr->AsInteger();
}

double FLCValue::AsDouble() const {
	return ValuePtr->AsDouble();
}

bool FLCValue::AsBoolean() const {
	return ValuePtr->AsBoolean();
}

TLCArray& FLCValue::AsArray() const {
	return ValuePtr->AsArray();
}

TLCMap& FLCValue::AsMap() const {
	return ValuePtr->AsMap();
}

FDateTime FLCValue::AsDate() const {
	return ValuePtr->AsDate();
}

FLCGeoPoint FLCValue::AsGeoPoint() const {
	return ValuePtr->AsGeoPoint();
}

TSharedPtr<FLCObject> FLCValue::AsObject() const {
	return ValuePtr->AsObject();
}

TArray<uint8> FLCValue::AsData() const {
	return ValuePtr->AsData();
}

FArchive& operator<<(FArchive& Ar, FLCValue& InValue) {
	if (Ar.IsLoading()) {
		ELCValueType ValueType;
		Ar << ValueType;
		switch (ValueType) {
		case ELCValueType::None:
			InValue.ValuePtr = MakeShared<FLCValueNone>();
			break;
		case ELCValueType::String: {
			FString RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueString>(RealValue);
		}
			break;
		case ELCValueType::Integer: {
			int64 RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueInteger>(RealValue);
		}
			break;
		case ELCValueType::Double: {
			double RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueDouble>(RealValue);
		}
			break;
		case ELCValueType::Boolean: {
			bool RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueBoolean>(RealValue);
		}
			break;
		case ELCValueType::Array: {
			TLCArray RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueArray>(RealValue);
		}
			break;
		case ELCValueType::Map: {
			TLCMap RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueMap>(RealValue);
		}
			break;
		case ELCValueType::Date: {
			FDateTime RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueDate>(RealValue);
		}
			break;
		case ELCValueType::GeoPoint: {
			FLCGeoPoint RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueGeoPoint>(RealValue);
		}
			break;
		case ELCValueType::Data: {
			TArray<uint8> RealValue;
			Ar << RealValue;
			InValue.ValuePtr = MakeShared<FLCValueData>(RealValue);
		}
			break;
		case ELCValueType::Object: {
			TSharedPtr<FLCObject> RealValue = MakeShared<FLCObject>("");
			Ar << *RealValue.Get();
			InValue.ValuePtr = MakeShared<FLCValueObject>(RealValue);
		}
			break;
		}

	} else {
		Ar << InValue.ValuePtr->ValueType;
		switch (InValue.ValuePtr->ValueType) {
		case ELCValueType::None:
			break;
		case ELCValueType::String: {
			Ar << StaticCastSharedPtr<FLCValueString>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Integer: {
			Ar << StaticCastSharedPtr<FLCValueInteger>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Double: {
			Ar << StaticCastSharedPtr<FLCValueDouble>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Boolean: {
			Ar << StaticCastSharedPtr<FLCValueBoolean>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Array: {
			Ar << StaticCastSharedPtr<FLCValueArray>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Map: {
			Ar << StaticCastSharedPtr<FLCValueMap>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Date: {
			Ar << StaticCastSharedPtr<FLCValueDate>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::GeoPoint: {
			Ar << StaticCastSharedPtr<FLCValueGeoPoint>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Data: {
			Ar << StaticCastSharedPtr<FLCValueData>(InValue.ValuePtr)->Value;
		}
			break;
		case ELCValueType::Object: {
			Ar << *StaticCastSharedPtr<FLCValueObject>(InValue.ValuePtr)->Value.Get();
		}
			break;
		}
	}
	return Ar;
}

FLCValue::~FLCValue() {
	
}

void FLCValue::SetStringValue(const FString& InValue) {
	ValuePtr = MakeShared<FLCValueString>(InValue);
}

void FLCValue::SetArrayValue(const TLCArray& InValue) {
	ValuePtr = MakeShared<FLCValueArray>(InValue);
}
