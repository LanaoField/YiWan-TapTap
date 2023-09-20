#include "LCValueBase.h"

#include "Misc/Base64.h"

TLCArray& FLCValueBase::AsArray() {
	static TLCArray Temp;
	return Temp;
}

TLCMap& FLCValueBase::AsMap() {
	static TLCMap Temp;
	return Temp;
}

FLCValue FLCValueNone::GetLconValue()
{
	return FLCValue();
}

FLCValue FLCValueString::GetLconValue()
{
	return FLCValue(Value);
}

FString FLCValueString::AsString() {
	return Value;
}

int64 FLCValueString::AsInteger() {
	int64 Result = 0;
	LexFromString(Result, *Value);
	return Result;
}

double FLCValueString::AsDouble() {
	double Result = 0;
	LexFromString(Result, *Value);
	return Result;
}

bool FLCValueString::AsBoolean() {
	bool Result = false;
	LexFromString(Result, *Value);
	return Result;
}

FDateTime FLCValueString::AsDate() {
	FDateTime OutDateTime;
	FDateTime::ParseIso8601(*Value, OutDateTime);
	return OutDateTime;
}

TArray<uint8> FLCValueString::AsData() {
	TArray<uint8> Result;
	FBase64::Decode(Value, Result);
	return MoveTemp(Result);
}

bool FLCValueString::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::String) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueString>(Rhs)->Value;
}

FLCValue FLCValueInteger::GetLconValue()
{
	return FLCValue(Value);
}

FString FLCValueInteger::AsString() {
	return LexToString(Value);
}

int64 FLCValueInteger::AsInteger() {
	return Value;
}

double FLCValueInteger::AsDouble() {
	return Value;
}

bool FLCValueInteger::AsBoolean() {
	return Value != 0;
}

bool FLCValueInteger::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Integer) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueInteger>(Rhs)->Value;
}

FLCValue FLCValueDouble::GetLconValue()
{
	return FLCValue(Value);
}

FString FLCValueDouble::AsString() {
	return LexToString(Value);
}

int64 FLCValueDouble::AsInteger() {
	return Value;
}

double FLCValueDouble::AsDouble() {
	return Value;
}

bool FLCValueDouble::AsBoolean() {
	return Value != 0.0;
}

bool FLCValueDouble::operator==(const TSharedPtr<FLCValueBase>& Rhs)
{
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Double) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueDouble>(Rhs)->Value;
}

FLCValue FLCValueBoolean::GetLconValue()
{
	return FLCValue(Value);
}

FString FLCValueBoolean::AsString() {
	return LexToString(Value);
}

int64 FLCValueBoolean::AsInteger() {
	return Value;
}

double FLCValueBoolean::AsDouble() {
	return Value;
}

bool FLCValueBoolean::AsBoolean() {
	return Value;
}

bool FLCValueBoolean::operator==(const TSharedPtr<FLCValueBase>& Rhs)
{
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Boolean) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueBoolean>(Rhs)->Value;
}

FLCValue FLCValueArray::GetLconValue()
{
	TLCArray Array;
	for (auto FlcValue : Value)
	{
		Array.Add(FlcValue.GetLconValue());
	}
	return FLCValue(Array);
}

TLCArray& FLCValueArray::AsArray() {
	return Value;
}

bool FLCValueArray::operator==(const TSharedPtr<FLCValueBase>& Rhs)
{
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Array) {
		return false;
	}
	auto RhsValue = StaticCastSharedPtr<FLCValueArray>(Rhs)->Value;
	if (RhsValue.Num() != Value.Num()) {
		return false;
	}
	for (int i = 0; i < RhsValue.Num(); ++i)
	{
		if (RhsValue[i] != Value[i]) {
			return false;
		}
	}
	return true;
}

FLCValue FLCValueMap::GetLconValue()
{
	TLCMap Map;
	for (auto FlcValue : Value)
	{
		Map.Add(FlcValue.Key, FlcValue.Value.GetLconValue());
	}
	return FLCValue(Map);
}

TLCMap& FLCValueMap::AsMap() {
	return Value;
}

bool FLCValueMap::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Map) {
		return false;
	}
	auto RhsValue = StaticCastSharedPtr<FLCValueMap>(Rhs)->Value;
	if (RhsValue.Num() != Value.Num()) {
		return false;
	}
	for (auto Result : RhsValue) {
		auto Ptr = Value.Find(Result.Key);
		if (Ptr == nullptr) {
			return false;
		}
		if (Result.Value != *Ptr) {
			return false;
		}
	}
	return true;
}

FLCValue FLCValueObject::GetLconValue()
{
	FString ObjectID = Value->GetObjectId();
	if (ObjectID.IsEmpty())
	{
		return FLCValue();
	} else
	{
		TLCMap Map;
		Map.Add("__type", "Pointer");
		Map.Add("className", Value->GetClassName());
		Map.Add("objectId", ObjectID);
		return FLCValue(Map);
	}
}

TSharedPtr<FLCObject> FLCValueObject::AsObject() {
	return Value;
}

bool FLCValueObject::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Object) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueObject>(Rhs)->Value;
}

FLCValue FLCValueDate::GetLconValue()
{
	TLCMap Map;
	Map.Add("__type", "Date");
	Map.Add("iso", Value.ToIso8601());
	return FLCValue(Map);
}
// ParseIso8601

FDateTime FLCValueDate::AsDate() {
	return Value;
}

FString FLCValueDate::AsString() {
	return Value.ToIso8601();
}

bool FLCValueDate::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Date) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueDate>(Rhs)->Value;
}

FLCValue FLCValueGeoPoint::GetLconValue()
{
	TLCMap Map;
	Map.Add("__type", "GeoPoint");
	Map.Add("latitude", Value.Latitude);
	Map.Add("longitude", Value.Longitude);
	return FLCValue(Map);
}

FLCGeoPoint FLCValueGeoPoint::AsGeoPoint() {
	return Value;
}

bool FLCValueGeoPoint::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::GeoPoint) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueGeoPoint>(Rhs)->Value;
}

FLCValue FLCValueData::GetLconValue()
{
	TLCMap Map;
	Map.Add("__type", "Bytes");
	Map.Add("base64", FBase64::Encode(Value));
	return FLCValue(Map);
}

// TArray<uint8> TUCrypto::Base64Decode(const FString& content)
// {
// 	TArray<uint8> outData;
// 	FBase64::Decode(content, outData);
// 	return outData;
// }

TArray<uint8> FLCValueData::AsData() {
	return Value;
}

FString FLCValueData::AsString() {
	return FBase64::Encode(Value);
}

bool FLCValueData::operator==(const TSharedPtr<FLCValueBase>& Rhs) {
	if (!Rhs.IsValid() || Rhs->ValueType != ELCValueType::Data) {
		return false;
	}
	return Value == StaticCastSharedPtr<FLCValueData>(Rhs)->Value;
}
