#pragma once
#include "LCError.h"
#if PLATFORM_IOS
#import <Foundation/Foundation.h>
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <LeanCloudObjc/LCUser.h>
#elif PLATFORM_ANDROID
#include "Android/LCJNI.h"
#define LeanCloudUE "com/lc/LeanCloudUE"
#endif

DECLARE_DELEGATE_TwoParams(FLeanCloudBoolResultDelegate, bool bIsSuccess, const FLCError& Error);

class LEANCLOUDMOBILE_API FLCObject : public TSharedFromThis<FLCObject>{
public:

#if PLATFORM_IOS
	FLCObject(LCObject *Object) : _Object(Object) {
		CFRetain((__bridge CFTypeRef)_Object);
	};
	FLCObject(const FLCObject& OtherObject) {
		_Object = OtherObject._Object;
		CFRetain((__bridge CFTypeRef)_Object);
	}
	FLCObject(FLCObject&& OtherObject) {
		_Object = OtherObject._Object;
		OtherObject._Object = nil;
	}

#elif PLATFORM_ANDROID
	FLCObject(jobject Object) {
		LCJNI::JNI LCJNI;
		_Object = (*LCJNI)->NewGlobalRef(Object);
	};
	FLCObject(const LCJNI::Object& Object) : FLCObject(*Object) {};
#endif
	virtual ~FLCObject();

	FString GetClassName() const;
	FDateTime GetCreatedAt() const;
	FDateTime GetUpdatedAt() const;
	FString GetObjectId() const;

	void SetStringForKey(const FString& InKey, const FString& InValue);
	void SetDoubleForKey(const FString& InKey, double InValue);
	void SetIntergerForKey(const FString& InKey, int64 InValue);
	void SetBooleanForKey(const FString& InKey, bool InValue);
	FString GetStringFromKey(const FString& InKey) const;
	double GetDoubleFromKey(const FString& InKey) const;
	int64 GetIntergerFromKey(const FString& InKey) const;
	bool GetBooleanFromKey(const FString& InKey) const;
	void DeleteValueForKey(const FString& InKey);


	void Save(FLeanCloudBoolResultDelegate CallBack = nullptr);

	TSharedPtr<FJsonObject> GetServerData() const;

protected:

	static FString KeyCreatedAt;
	static FString KeyUpdateAt;
	static FString KeyObjectID;


#if PLATFORM_IOS
	LCObject *_Object;
#elif PLATFORM_ANDROID
	jobject _Object;
#endif

};
