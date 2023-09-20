#pragma once
#include "LCApplication.h"
#include "LCError.h"
#include "LCSaveOption.h"
#include "LCValue.h"

DECLARE_DELEGATE_TwoParams(FLeanCloudBoolResultDelegate, bool bIsSuccess, const FLCError& Error);


class LEANCLOUD_API FLCObject : public TSharedFromThis<FLCObject> {
public:
	void SetApplicationPtr(const TSharedPtr<FLCApplication>& InPtr);
	TSharedPtr<FLCApplication> GetApplicationPtr() const;

	FLCObject(const FString& InClassName);
	FLCObject(const FString& InClassName, const FString& InObjectId);
	FLCObject(const FString& InClassName, const TLCMap& InServerData);
	virtual ~FLCObject();

	void Set(const FString& Key, const FLCValue& Value);
	void Unset(const FString& Key);
	FLCValue Get(const FString& Key) const;

	void Increase(const FString& Key, int Value = 1);
	void Increase(const FString& Key, double Value = 1);


	/**
	 * @brief 将指定对象或数组附加到数组末尾
	 * @param Key 数组所在的Key
	 * @param Value 要添加的对象
	 * @param bIsUnique 确保对象唯一
	 */
	void Add(const FString& Key, const FLCValue& Value, bool bIsUnique = false);

	/**
	 * @brief 从数组字段中删除指定对象或数组的所有实例
	 * @param Key 数组所在的Key
	 * @param Value 要删除的对象
	 */
	void Remove(const FString& Key, const FLCValue& Value);

	// 序列化
	FString ToString();
	static TSharedPtr<FLCObject> Parse(const FString& ObjectString);

	FLCSaveOption GenerateSaveOption() const;

	void Save(FLeanCloudBoolResultDelegate CallBack = nullptr);
	void Save(const FLCSaveOption& Option, FLeanCloudBoolResultDelegate CallBack = nullptr);
	void Fetch(FLeanCloudBoolResultDelegate CallBack = nullptr);
	void Fetch(const TArray<FString>& Keys, FLeanCloudBoolResultDelegate CallBack = nullptr);
	void Delete(FLeanCloudBoolResultDelegate CallBack = nullptr);
	static void Save(const TArray<TSharedPtr<FLCObject>>& Objects, FLeanCloudBoolResultDelegate CallBack = nullptr);
	static void Save(const TArray<TSharedPtr<FLCObject>>& Objects, const FLCSaveOption& Option,
	                 FLeanCloudBoolResultDelegate CallBack = nullptr);
	static void Fetch(const TArray<TSharedPtr<FLCObject>>& Objects, FLeanCloudBoolResultDelegate CallBack = nullptr);
	static void Fetch(const TArray<TSharedPtr<FLCObject>>& Objects, const TArray<FString>& Keys,
	                  FLeanCloudBoolResultDelegate CallBack = nullptr);
	static void Delete(const TArray<TSharedPtr<FLCObject>>& Objects, FLeanCloudBoolResultDelegate CallBack = nullptr);

	FString GetClassName() const;
	FDateTime GetCreatedAt() const;
	FDateTime GetUpdatedAt() const;
	FString GetObjectId() const;
	TLCMap GetServerData() const;
	FString GetInternalId();

	friend FArchive& operator<<(FArchive& Ar, FLCObject& Object);

	template <typename SubClass>
	static TSharedPtr<SubClass> CreateObject(const TLCMap& InServerData, const TSharedPtr<FLCApplication>& InPtr) {
		if (InServerData.Num() == 0) {
			return nullptr;
		}
		TSharedPtr<SubClass> Ptr = MakeShared<SubClass>();
		Ptr->UpdateDataFromServer(InServerData);
		Ptr->SetApplicationPtr(InPtr);
		return MoveTemp(Ptr);
	}

	template <typename SubClass>
	TSharedPtr<SubClass> ConvertTo() {
		if (ServerData.Num() == 0) {
			return nullptr;
		}
		TSharedPtr<SubClass> Ptr = MakeShared<SubClass>();
		Ptr->UpdateDataFromServer(ServerData);
		Ptr->SetApplicationPtr(GetApplicationPtr());
		return MoveTemp(Ptr);
	}

protected:
	void UpdateDataFromServer(const TLCMap& InServerData);

private:
	friend class FLCObjectUpdater;
	friend class FLCQuery;
	TWeakPtr<FLCApplication> ApplicationPtr;
	TLCMap ServerData;
	TLCMap Operations;
	FString _InternalId;
	void ClearOperations();
};
