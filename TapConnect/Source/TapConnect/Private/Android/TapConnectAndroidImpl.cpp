#include "TapConnectAndroidImpl.h"
#include "TapJNI.h"

#define TapConnectUE "com/tds/TapConnectUE"

FTapConnectAndroidImpl::~FTapConnectAndroidImpl() {
}

void FTapConnectAndroidImpl::Init(const FString& ClientID, const FString& ClientToken, bool bIsCN)
{
	TapJNI::JNI JNI;
	auto ClassObject = JNI.FindClass(TapConnectUE);
	JNI.CallStaticVoidMethod(ClassObject, "init",
							 "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Z)V",
							 *JNI.GetActivity(), *JNI.ToJavaString(ClientID), *JNI.ToJavaString(ClientToken), bIsCN);
}

void FTapConnectAndroidImpl::SetEntryVisible(bool bVisible) {
	TapJNI::JNI JNI;
	auto ClassObject = JNI.FindClass(TapConnectUE);
	JNI.CallStaticVoidMethod(ClassObject, "setEntryVisible",
							 "(Z)V", bVisible);
}
