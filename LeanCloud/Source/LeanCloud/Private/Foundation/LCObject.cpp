#include "LCObject.h"

#include "LCObjectUpdater.h"
#include "Tools/LCJsonHelper.h"
static FString KeyClassName = "className";
static FString KeyCreatedAt = "createdAt";
static FString KeyUpdateAt = "updatedAt";
static FString KeyObjectID = "objectId";
static FString KeyACL = "ACL";
static FString KeyIgnoreHooks = "__ignore_hooks";

void FLCObject::SetApplicationPtr(const TSharedPtr<FLCApplication>& InPtr) {
	ApplicationPtr = InPtr;
}

TSharedPtr<FLCApplication> FLCObject::GetApplicationPtr() const {
	if (ApplicationPtr.IsValid()) {
		return ApplicationPtr.Pin();
	}
	else {
		return FLCApplication::DefaultPtr;
	}
}

// FLCObject::FLCObject(const FString& InClassName, const TLCMap& InServerData) {
// 	ServerData = TLCMap(InServerData);
// 	ServerData.Add(KeyClassName, InClassName);
// }

FLCObject::FLCObject(const FString& InClassName) {
	ServerData.Add(KeyClassName, InClassName);
}

// FLCObject::FLCObject(const TLCMap& InServerData) {
// 	ServerData = TLCMap(InServerData);
// }

FLCObject::FLCObject(const FString& InClassName, const FString& InObjectId) {
	ServerData.Add(KeyClassName, InClassName);
	ServerData.Add(KeyObjectID, InObjectId);
}

FLCObject::FLCObject(const FString& InClassName, const TLCMap& InServerData) {
	ServerData = InServerData;
	ServerData.Add(KeyClassName, InClassName);
}

FLCObject::~FLCObject() {
}

void FLCObject::Set(const FString& Key, const FLCValue& Value) {
	if (Value.IsNoneType()) {
		Unset(Key);
	} else {
		ServerData.Add(Key, Value);
		Operations.Add(Key, Value);
	}
}

void FLCObject::Unset(const FString& Key) {
	ServerData.Remove(Key);
	TLCMap DeleteOp;
	DeleteOp.Add("__op", "Delete");
	DeleteOp.Add("delete", true);
	Operations.Add(Key, DeleteOp);
}

FLCValue FLCObject::Get(const FString& Key) const {
	const FLCValue *ValuePtr = ServerData.Find(Key);
	if (ValuePtr == nullptr) {
		return FLCValue();
	} else {
		return *ValuePtr;
	}
}

void FLCObject::Increase(const FString& Key, int Value) {
	FLCValue OldValue = Get(Key);
	if (OldValue.IsNoneType()) {
		ServerData.Add(Key, Value);
	} else if (OldValue.IsIntegerType()) {
		ServerData.Add(Key, OldValue.AsInteger() + Value);
	} else if (OldValue.IsDoubleType()) {
		ServerData.Add(Key, OldValue.AsDouble() + Value);
	}
	TLCMap IncreaseOp;
	IncreaseOp.Add("__op", "Increment");
	IncreaseOp.Add("amount", Value);
	Operations.Add(Key, IncreaseOp);
}

void FLCObject::Increase(const FString& Key, double Value) {
	FLCValue OldValue = Get(Key);
	if (OldValue.IsNoneType()) {
		ServerData.Add(Key, Value);
	} else if (OldValue.IsIntegerType()) {
		ServerData.Add(Key, OldValue.AsInteger() + Value);
	} else if (OldValue.IsDoubleType()) {
		ServerData.Add(Key, OldValue.AsDouble() + Value);
	}
	TLCMap IncreaseOp;
	IncreaseOp.Add("__op", "Increment");
	IncreaseOp.Add("amount", Value);
	Operations.Add(Key, IncreaseOp);
}

void FLCObject::Add(const FString& Key, const FLCValue& Value, bool bIsUnique) {
	TArray<FString> s;
	s.Contains("");
}

void FLCObject::Remove(const FString& Key, const FLCValue& Value) {
}

FString FLCObject::ToString() {
	if (DoesSharedInstanceExist()) {
		return FLCJsonHelper::GetJsonString(this->AsShared());
	}
	else {
		return FLCJsonHelper::GetJsonString(MakeShared<FLCObject>(*this));
	}
}

TSharedPtr<FLCObject> FLCObject::Parse(const FString& ObjectString) {
	return nullptr;
}

FLCSaveOption FLCObject::GenerateSaveOption() const {
	FLCQuery Query = FLCQuery(GetClassName(), GetApplicationPtr());
	return FLCSaveOption(Query);
}

void FLCObject::Save(FLeanCloudBoolResultDelegate CallBack) {
	TArray<TSharedPtr<FLCObject>> TempArr;
	if (DoesSharedInstanceExist()) {
		TempArr.Add(this->AsShared());
	}
	else {
		TempArr.Add(MakeShared<FLCObject>(*this));
	}
	Save(TempArr, MoveTemp(CallBack));
}

void FLCObject::Save(const FLCSaveOption& Option, FLeanCloudBoolResultDelegate CallBack) {
	TArray<TSharedPtr<FLCObject>> TempArr;
	if (DoesSharedInstanceExist()) {
		TempArr.Add(this->AsShared());
	}
	else {
		TempArr.Add(MakeShared<FLCObject>(*this));
	}
	Save(TempArr, Option, MoveTemp(CallBack));
}

void FLCObject::Fetch(FLeanCloudBoolResultDelegate CallBack) {
	TArray<TSharedPtr<FLCObject>> TempArr;
	if (DoesSharedInstanceExist()) {
		TempArr.Add(this->AsShared());
	}
	else {
		TempArr.Add(MakeShared<FLCObject>(*this));
	}
	Fetch(TempArr, MoveTemp(CallBack));
}

void FLCObject::Fetch(const TArray<FString>& Keys, FLeanCloudBoolResultDelegate CallBack) {
	TArray<TSharedPtr<FLCObject>> TempArr;
	if (DoesSharedInstanceExist()) {
		TempArr.Add(this->AsShared());
	}
	else {
		TempArr.Add(MakeShared<FLCObject>(*this));
	}
	Fetch(TempArr, Keys, MoveTemp(CallBack));
}

void FLCObject::Delete(FLeanCloudBoolResultDelegate CallBack) {
	TArray<TSharedPtr<FLCObject>> TempArr;
	if (DoesSharedInstanceExist()) {
		TempArr.Add(this->AsShared());
	}
	else {
		TempArr.Add(MakeShared<FLCObject>(*this));
	}
	Delete(TempArr, MoveTemp(CallBack));
}

void FLCObject::Save(const TArray<TSharedPtr<FLCObject>>& Objects, FLeanCloudBoolResultDelegate CallBack) {
	FLCObjectUpdater::Save(Objects, MoveTemp(CallBack));
}

void FLCObject::Save(const TArray<TSharedPtr<FLCObject>>& Objects, const FLCSaveOption& Option,
	FLeanCloudBoolResultDelegate CallBack) {
	FLCObjectUpdater::Save(Objects, Option, MoveTemp(CallBack));
}

void FLCObject::Fetch(const TArray<TSharedPtr<FLCObject>>& Objects, FLeanCloudBoolResultDelegate CallBack) {
	Fetch(Objects, {}, MoveTemp(CallBack));
}

void FLCObject::Fetch(const TArray<TSharedPtr<FLCObject>>& Objects, const TArray<FString>& Keys,
	FLeanCloudBoolResultDelegate CallBack) {
	FLCObjectUpdater::Fetch(Objects, Keys, MoveTemp(CallBack));
}

void FLCObject::Delete(const TArray<TSharedPtr<FLCObject>>& Objects, FLeanCloudBoolResultDelegate CallBack) {
	FLCObjectUpdater::Delete(Objects, MoveTemp(CallBack));
}

FString FLCObject::GetClassName() const {
	return Get(KeyClassName).AsString();
}

FDateTime FLCObject::GetCreatedAt() const {
	return Get(KeyCreatedAt).AsDate();
}

FDateTime FLCObject::GetUpdatedAt() const {
	return Get(KeyUpdateAt).AsDate();
}

FString FLCObject::GetObjectId() const {
	return Get(KeyObjectID).AsString();;
}

TLCMap FLCObject::GetServerData() const {
	// FMemoryReader
	return ServerData;
}

FString FLCObject::GetInternalId() {
	FString InternalId = GetObjectId();
	if (!InternalId.IsEmpty()) {
		return InternalId;
	}
	if (_InternalId.IsEmpty()) {
		_InternalId = FGuid::NewGuid().ToString();
	}
	return _InternalId;
}

void FLCObject::ClearOperations() {
	Operations.Empty();
}

void FLCObject::UpdateDataFromServer(const TLCMap& InServerData) {
	for (auto SubData : InServerData) {
		ServerData.Add(SubData.Key, SubData.Value.GetFromLconValue());
	}
}

// 预留，万一以后Object的结构有变化
static int ObjectVersion = 1;

FArchive& operator<<(FArchive& Ar, FLCObject& Object) {
	int Version = ObjectVersion;
	if (Ar.IsLoading()) {
		Ar << Version;
		// if (Version == ObjectVersion) {
		// 	
		// } else if (Version == 1) {
		// 	
		// }
		return Ar << Object.ServerData << Object.Operations << Object._InternalId;
	} else {
		return Ar << Version << Object.ServerData << Object.Operations << Object._InternalId;
	}
}
