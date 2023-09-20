#include "LCJsonHelper.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

// 如果ObjectType，出现相互指向，会产生死循环。这里只是输出JSON，最好都是JSON支持的类型，特殊的类型不要出现在这，虽然这里也做了处理
FString FLCJsonHelper::GetJsonString(const TLCMap& Value, bool bNeedNull) {
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	// typedef  TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> FDiffJsonWriter;
	// typedef  TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> FDiffJsonWriterFactory;
	JsonWriter->WriteObjectStart();
	for (auto Tuple : Value) {
		if (Tuple.Value.IsIntegerType()) {
			JsonWriter->WriteValue(Tuple.Key, Tuple.Value.AsInteger());
		}
		else if (Tuple.Value.IsDoubleType()) {
			JsonWriter->WriteValue(Tuple.Key, Tuple.Value.AsDouble());
		}
		else if (Tuple.Value.IsBooleanType()) {
			JsonWriter->WriteValue(Tuple.Key, Tuple.Value.AsBoolean());
		}
		else if (Tuple.Value.IsStringType() ||
			Tuple.Value.IsDataType() ||
			Tuple.Value.IsDateType()) {
			JsonWriter->WriteValue(Tuple.Key, Tuple.Value.AsString());
		}
		else if (bNeedNull && Tuple.Value.IsNoneType()) {
			JsonWriter->WriteNull(Tuple.Key);
		}
		else if (Tuple.Value.IsMapType()) {
			JsonWriter->WriteRawJSONValue(Tuple.Key, GetJsonString(Tuple.Value.AsMap(), bNeedNull));
		}
		else if (Tuple.Value.IsObjectType()) {
			JsonWriter->WriteRawJSONValue(Tuple.Key, GetJsonString(Tuple.Value.AsObject(), bNeedNull));
		}
		else if (Tuple.Value.IsArrayType()) {
			JsonWriter->WriteRawJSONValue(Tuple.Key, GetJsonString(Tuple.Value.AsArray(), bNeedNull));
		}
		else if (Tuple.Value.IsGeoPointType()) {
			JsonWriter->WriteObjectStart(Tuple.Key);
			auto GeoPoint = Tuple.Value.AsGeoPoint();
			JsonWriter->WriteValue("latitude", GeoPoint.Latitude);
			JsonWriter->WriteValue("longitude", GeoPoint.Longitude);
			JsonWriter->WriteObjectEnd();
		}
	}
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	return JsonStr;
}

FString FLCJsonHelper::GetJsonString(const TLCArray& Value, bool bNeedNull) {
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteArrayStart();
	for (auto SubValue : Value) {
		if (SubValue.IsIntegerType()) {
			JsonWriter->WriteValue(SubValue.AsInteger());
		}
		else if (SubValue.IsDoubleType()) {
			JsonWriter->WriteValue(SubValue.AsDouble());
		}
		else if (SubValue.IsBooleanType()) {
			JsonWriter->WriteValue(SubValue.AsBoolean());
		}
		else if (SubValue.IsStringType() ||
			SubValue.IsDataType() ||
			SubValue.IsDateType()) {
			JsonWriter->WriteValue(SubValue.AsString());
		}
		else if (bNeedNull && SubValue.IsNoneType()) {
			JsonWriter->WriteNull();
		}
		else if (SubValue.IsMapType() || SubValue.IsGeoPointType()) {
			JsonWriter->WriteRawJSONValue(GetJsonString(SubValue.AsMap(), bNeedNull));
		}
		else if (SubValue.IsObjectType()) {
			JsonWriter->WriteRawJSONValue(GetJsonString(SubValue.AsObject(), bNeedNull));
		}
		else if (SubValue.IsArrayType()) {
			JsonWriter->WriteRawJSONValue(GetJsonString(SubValue.AsArray(), bNeedNull));
		}
		else if (SubValue.IsGeoPointType()) {
			JsonWriter->WriteObjectStart();
			auto GeoPoint = SubValue.AsGeoPoint();
			JsonWriter->WriteValue("latitude", GeoPoint.Latitude);
			JsonWriter->WriteValue("longitude", GeoPoint.Longitude);
			JsonWriter->WriteObjectEnd();
		}
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->Close();

	return JsonStr;
}

FString FLCJsonHelper::GetJsonString(const TSharedPtr<FLCObject>& Value, bool bNeedNull) {
	return GetJsonString(Value->GetServerData(), bNeedNull);
}

struct StackState {
	FString Identifier;
	TSharedPtr<FLCValue> ValuePtr;
};

FLCValue FLCJsonHelper::GetJsonValue(const FString& JsonStr) {
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonStr);
	TArray<TSharedRef<StackState>> ScopeStack;
	TSharedPtr<StackState> CurrentState;
	TSharedPtr<FLCValue> NewValuePtr;
	EJsonNotation Notation;

	while (JsonReader->ReadNext(Notation)) {
		FString Identifier = JsonReader->GetIdentifier();
		NewValuePtr.Reset();

		switch (Notation) {
		case EJsonNotation::ObjectStart: {
			if (CurrentState.IsValid()) {
				ScopeStack.Push(CurrentState.ToSharedRef());
			}

			CurrentState = MakeShared<StackState>();
			CurrentState->Identifier = Identifier;
			CurrentState->ValuePtr = MakeShared<FLCValue>(TLCMap());
		}
		break;

		case EJsonNotation::ObjectEnd: {
			if (ScopeStack.Num() > 0) {
				Identifier = CurrentState->Identifier;
				NewValuePtr = CurrentState->ValuePtr;
				CurrentState = ScopeStack.Pop();
			}
		}
		break;

		case EJsonNotation::ArrayStart: {
			if (CurrentState.IsValid()) {
				ScopeStack.Push(CurrentState.ToSharedRef());
			}

			CurrentState = MakeShared<StackState>();
			CurrentState->Identifier = Identifier;
			CurrentState->ValuePtr = MakeShared<FLCValue>(TLCArray());
		}
		break;

		case EJsonNotation::ArrayEnd: {
			if (ScopeStack.Num() > 0) {
				Identifier = CurrentState->Identifier;
				NewValuePtr = CurrentState->ValuePtr;
				CurrentState = ScopeStack.Pop();
			}
		}
		break;

		case EJsonNotation::Boolean:
			NewValuePtr = MakeShared<FLCValue>(JsonReader->GetValueAsBoolean());
			break;

		case EJsonNotation::String:
			NewValuePtr = MakeShared<FLCValue>(JsonReader->GetValueAsString());
			break;

		case EJsonNotation::Number: {
			FString NumStr = JsonReader->GetValueAsNumberString();
			if (NumStr.Contains(TEXT("."))) {
				double Num = 0;
				LexFromString(Num, *NumStr);
				NewValuePtr = MakeShared<FLCValue>(Num);
			}
			else {
				int64 Num = 0;
				LexFromString(Num, *NumStr);
				NewValuePtr = MakeShared<FLCValue>(Num);
			}
		}
			break;

		case EJsonNotation::Null:
			NewValuePtr = MakeShared<FLCValue>();
			break;

		case EJsonNotation::Error:
			return FLCValue();
			break;
		}

		if (NewValuePtr.IsValid() && CurrentState.IsValid() && CurrentState->ValuePtr.IsValid()) {
			if (CurrentState->ValuePtr->IsMapType()) {
				CurrentState->ValuePtr->AsMap().Add(Identifier, *NewValuePtr.Get());
			}
			else if (CurrentState->ValuePtr->IsArrayType()) {
				CurrentState->ValuePtr->AsArray().Add(*NewValuePtr.Get());
			}
		}
	}

	if (!CurrentState.IsValid() || !CurrentState->ValuePtr.IsValid() || !JsonReader->GetErrorMessage().IsEmpty()) {
		return FLCValue();
	}

	return *CurrentState->ValuePtr.Get();
}
