#include "TapBootstrapImplAndroid.h"

#include "TapBootstrap.h"
#include "TapJavaHelper.h"
#include "TapJNI.h"
#include "TUHelper.h"
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJNI.h"
#include "TapJNICallbackHelper.h"
#include "Android/TapBootstrapJavaHelper.h"

#define TapBootstrapUE "com/tds/TapBootstrapUE"

FTapBootstrapImplAndroid::~FTapBootstrapImplAndroid() {
}

void FTapBootstrapImplAndroid::Init(const FTUConfig& InConfig) {
	FTUConfig::Init(InConfig);
	TapJNI::JNI JNI;
	auto ClassObject = JNI.FindClass(TapBootstrapUE);
	auto Activity = JNI.GetActivity();
	auto ClientID = JNI.ToJavaString(InConfig.ClientID);
	auto ClientToken = JNI.ToJavaString(InConfig.ClientToken);
	auto ServerURL = JNI.ToJavaString(InConfig.ServerURL);
	auto Channel = JNI.ToJavaString(InConfig.DBConfig.Channel);
	auto GameVersion = JNI.ToJavaString(InConfig.DBConfig.GameVersion);
	bool IsCN = InConfig.RegionType == ERegionType::CN;
	bool DBConfigEnable = InConfig.DBConfig.Enable;
	bool bBillboardEnable = InConfig.BillboardConfig.IsValid();
	if (bBillboardEnable) {
		auto jBillboardUrl = JNI.ToJavaString(InConfig.BillboardConfig->BillboardUrl);

		JNIEnv*	JEnv = *JNI;
	
		auto jDimensionString = NewScopedJavaObject(JEnv, (jobjectArray)JEnv->NewObjectArray(InConfig.BillboardConfig->Dimensions.Num() * 2, FJavaWrapper::JavaStringClass, nullptr));
		int32 Index = 0;
		for (TTuple<FString, FString>& T : InConfig.BillboardConfig->Dimensions)
		{
			JEnv->SetObjectArrayElement(*jDimensionString, Index++, *FJavaHelper::ToJavaString(JEnv, T.Key));
			JEnv->SetObjectArrayElement(*jDimensionString, Index++, *FJavaHelper::ToJavaString(JEnv, T.Value));
		}
		JNI.CallStaticVoidMethod(ClassObject, "init", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZLjava/lang/String;Ljava/lang/String;ZLjava/lang/String;[Ljava/lang/String;)V", *Activity, *ClientID, *ClientToken, *ServerURL, IsCN, DBConfigEnable, *Channel, *GameVersion, bBillboardEnable, *jBillboardUrl, *jDimensionString);
	} else {
		JNI.CallStaticVoidMethod(ClassObject, "init", "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZLjava/lang/String;Ljava/lang/String;ZLjava/lang/String;[Ljava/lang/String;)V", *Activity, *ClientID, *ClientToken, *ServerURL, IsCN, DBConfigEnable, *Channel, *GameVersion, bBillboardEnable, NULL, NULL);
	}
	
	
}

void FTapBootstrapImplAndroid::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) {
	// int InCallBackID = GetCallBackID();
	// RankingsResultCallBacks.Add(InCallBackID, MakeTuple(OnSuccess, OnError));
	
	TapJNI::JNI JNI;
	auto ClassObject = JNI.FindClass(TapBootstrapUE);
	JNI.CallStaticVoidMethod(ClassObject, "queryTapFriendsLeaderBoard",
							 "(Ljava/lang/String;III)V",
							 *JNI.ToJavaString(Name), From, Limit, FTapJNICallbackHelper::AddCallBack(OnSuccess, OnError));
}


#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT void Java_com_tds_TapBootstrapUE_onRankingsError(JNIEnv* jenv, jclass thiz, jint code, jstring Message,
														   jint callBackID) {
		auto CallBackPtr = FTapJNICallbackHelper::FindCallBack<FTUError::FDelegate>(callBackID, 1);
		if (CallBackPtr == nullptr) {
			return;
		}
		TapJNI::JNI JNI(jenv);
		FString Msg_UE = JNI.GetFStringFromParam(Message);
		FTUError Error(code, Msg_UE);
		TUHelper::PerformOnGameThread([=]() {
			CallBackPtr->ExecuteIfBound(Error);
			FTapJNICallbackHelper::RemoveCallBack(callBackID);
		});
	}

	JNIEXPORT void Java_com_tds_TapBootstrapUE_onRankingsSuccess(JNIEnv* jenv, jclass thiz, jobject java_rankings, jint callBackID) {

		auto CallBackPtr = FTapJNICallbackHelper::FindCallBack<FTDSLeaderBoardRanking::FRankingsDelegate>(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		TapJNI::JNI JNI(jenv);
		TArray<FTDSLeaderBoardRanking> UE_Rankings;
		TapJavaHelper::ListForEach(JNI, TapJNI::MakeScopedJavaObject(*JNI, java_rankings, false), [=, &UE_Rankings](int Index, const TapJNI::Object& Object) {
			FTDSLeaderBoardRanking UE_Ranking;
			UE_Ranking.Rank = JNI.CallIntMethod(Object, "getRank", "()I");
			UE_Ranking.StatisticValue = JNI.CallIntMethod(Object, "getStatisticValue", "()I");
			auto StatisticName = JNI.CallObjectMethod(Object, "getStatisticName", "()Ljava/lang/String;");
			UE_Ranking.StatisticName = JNI.GetFString(StatisticName);
			auto User = JNI.CallObjectMethod(Object, "getUser", "()Lcom/tapsdk/bootstrap/account/TDSUser;");
			UE_Ranking.User = TapBootstrapJavaHelper::ConvertUser(User);
			UE_Rankings.Add(UE_Ranking);
		});
		TUHelper::PerformOnGameThread([=]() {
			CallBackPtr->ExecuteIfBound(UE_Rankings);
			FTapJNICallbackHelper::RemoveCallBack(callBackID);
		});
	}


#ifdef __cplusplus
}
#endif
