#include "LCObject.h"
#include "Tools/LCJsonHelper.h"
#include "Tools/LCHelper.h"

#if PLATFORM_IOS
#include "iOS/LCObjcHelper.h"
#elif PLATFORM_ANDROID
#include "Android/LCJavaHelper.h"
#include "Android/LCJNICallbackHelper.h"
#endif

FString FLCObject::KeyCreatedAt = "createdAt";
FString FLCObject::KeyUpdateAt = "updatedAt";
FString FLCObject::KeyObjectID = "objectId";

FLCObject::~FLCObject() {
#if PLATFORM_IOS
	if (_Object) {
		CFRelease((__bridge CFTypeRef)_Object);
	}
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	(*LCJNI)->DeleteGlobalRef(_Object);
#endif
}


FString FLCObject::GetClassName() const {
#if PLATFORM_IOS
	return LCObjcHelper::Convert([_Object className]);
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	auto JaveStr = LCJNI.CallStaticObjectMethod(LeanCloudUEClass, "GetClassName", "(Lcn/leancloud/LCObject;)Ljava/lang/String;", _Object);
	return LCJNI.GetFString(JaveStr);
#endif
	return "";
}

FDateTime FLCObject::GetCreatedAt() const {
#if PLATFORM_IOS
	return LCObjcHelper::Convert(_Object.createdAt);
#endif
	FString TimeStr = GetStringFromKey(KeyCreatedAt);
	FDateTime DateTime;
	FDateTime::ParseIso8601(*TimeStr, DateTime);
	return DateTime;
}

FDateTime FLCObject::GetUpdatedAt() const {
#if PLATFORM_IOS
	return LCObjcHelper::Convert(_Object.updatedAt);
#endif
	FString TimeStr = GetStringFromKey(KeyUpdateAt);
	FDateTime DateTime;
	FDateTime::ParseIso8601(*TimeStr, DateTime);
	return DateTime;
}

FString FLCObject::GetObjectId() const {
	return GetStringFromKey(KeyObjectID);
}

void FLCObject::SetStringForKey(const FString& InKey, const FString& InValue) {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	[_Object setObject:LCObjcHelper::Convert(InValue) forKey:iOS_Key];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "SetStringForKey",
	"(Lcn/leancloud/LCObject;Ljava/lang/String;Ljava/lang/String;)V",
	_Object, *LCJNI.ToJavaString(InKey), *LCJNI.ToJavaString(InValue));
#endif
}

void FLCObject::SetDoubleForKey(const FString& InKey, double InValue) {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	[_Object setObject:@(InValue) forKey:iOS_Key];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "SetDoubleForKey",
	"(Lcn/leancloud/LCObject;Ljava/lang/String;D)V",
	_Object, *LCJNI.ToJavaString(InKey), InValue);
#endif
}

void FLCObject::SetIntergerForKey(const FString& InKey, int64 InValue) {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	[_Object setObject:@(InValue) forKey:iOS_Key];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "SetIntergerForKey",
	"(Lcn/leancloud/LCObject;Ljava/lang/String;J)V",
	_Object, *LCJNI.ToJavaString(InKey), InValue);
#endif
}

void FLCObject::SetBooleanForKey(const FString& InKey, bool InValue) {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	[_Object setObject:@(InValue) forKey:iOS_Key];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "SetBooleanForKey",
	"(Lcn/leancloud/LCObject;Ljava/lang/String;Z)V",
	_Object, *LCJNI.ToJavaString(InKey), InValue);
#endif
}

FString FLCObject::GetStringFromKey(const FString& InKey) const {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	NSString *stringValue = [_Object objectForKey:iOS_Key];
	if ([stringValue isKindOfClass:[NSString class]]) {
		return LCObjcHelper::Convert(stringValue);
	} 
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	auto JaveStr = LCJNI.CallStaticObjectMethod(LeanCloudUEClass, "GetStringFromKey",
		"(Lcn/leancloud/LCObject;Ljava/lang/String;)Ljava/lang/String;",
		_Object, *LCJNI.ToJavaString(InKey));
	return LCJNI.GetFString(JaveStr);
#endif
	return "";
}

double FLCObject::GetDoubleFromKey(const FString& InKey) const {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	NSNumber *numValue = [_Object objectForKey:iOS_Key];
	if ([numValue isKindOfClass:[NSNumber class]]) {
		return [numValue doubleValue];
	}
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	return LCJNI.CallStaticDoubleMethod(LeanCloudUEClass, "GetDoubleFromKey",
		"(Lcn/leancloud/LCObject;Ljava/lang/String;)D",
		_Object, *LCJNI.ToJavaString(InKey));
#endif
	return 0;
}

int64 FLCObject::GetIntergerFromKey(const FString& InKey) const {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	NSNumber *numValue = [_Object objectForKey:iOS_Key];
	if ([numValue isKindOfClass:[NSNumber class]]) {
		return [numValue integerValue];
	}
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	return LCJNI.CallStaticLongMethod(LeanCloudUEClass, "GetIntergerFromKey",
		"(Lcn/leancloud/LCObject;Ljava/lang/String;)J",
		_Object, *LCJNI.ToJavaString(InKey));
#endif
	return 0;
}

bool FLCObject::GetBooleanFromKey(const FString& InKey) const {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	NSNumber *numValue = [_Object objectForKey:iOS_Key];
	if ([numValue isKindOfClass:[NSNumber class]]) {
		return [numValue boolValue];
	}
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	return LCJNI.CallStaticBooleanMethod(LeanCloudUEClass, "GetBooleanFromKey",
	"(Lcn/leancloud/LCObject;Ljava/lang/String;)Z",
	_Object, *LCJNI.ToJavaString(InKey));
#endif
	return false;
}

void FLCObject::DeleteValueForKey(const FString& InKey) {
#if PLATFORM_IOS
	NSString *iOS_Key = LCObjcHelper::Convert(InKey);
	[_Object setObject:nil forKey:iOS_Key];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "DeleteValueForKey",
	"(Lcn/leancloud/LCObject;Ljava/lang/String;)V",
	_Object, *LCJNI.ToJavaString(InKey));
#endif
}

TSharedPtr<FJsonObject> FLCObject::GetServerData() const {
#if PLATFORM_IOS
	return LCObjcHelper::Convert((NSDictionary *)[_Object dictionaryForObject]);
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	auto JaveMap = LCJNI.CallStaticObjectMethod(LeanCloudUEClass, "getServerData",
		"(Lcn/leancloud/LCObject;)Ljava/util/Map;", _Object);
	return LCJsonHelper::GetJsonObject(LCJavaHelper::GetJsonFromMap(LCJNI, JaveMap));
#endif
	return nullptr;
}

void FLCObject::Save(FLeanCloudBoolResultDelegate CallBack) {
#if PLATFORM_IOS
	[_Object saveInBackgroundWithBlock:^(BOOL succeeded, NSError * _Nullable error) {
		auto UE_Error = LCObjcHelper::Convert(error);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(succeeded, UE_Error);
		});
	}];
#elif PLATFORM_ANDROID
	LCJNI::JNI LCJNI;
	auto LeanCloudUEClass = LCJNI.FindClass(LeanCloudUE);
	LCJNI.CallStaticVoidMethod(LeanCloudUEClass, "Save",
	"(Lcn/leancloud/LCObject;I)V",
	_Object, FLCJNICallbackHelper::AddCallBack(CallBack));
#endif
}

#if PLATFORM_ANDROID
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void Java_com_lc_LeanCloudUE_onLeanCloudBoolResultDelegate(JNIEnv* jenv, jclass thiz, jboolean success, jstring ErrorReason, jint callBackID) {
		auto CallBackPtr = FLCJNICallbackHelper::FindCallBack<FLeanCloudBoolResultDelegate>(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FLCError Error(-1, LCJNI.GetFStringFromParam(ErrorReason));
		LCHelper::PerformOnGameThread([=]() {
			CallBackPtr->ExecuteIfBound(success, Error);
			FLCJNICallbackHelper::RemoveCallBack(callBackID);
		});
	}

#ifdef __cplusplus
}
#endif
#endif