#pragma once
#include "LCError.h"
#include "LCUser.h"
#include "TUError.h"
// #if PLATFORM_ANDROID
// #include "TapJNI.h"
// #endif

/**
 * TapTap Developer Services user
 */
/* Old -> New
 * uuid -> GetObjectId()
 * userName -> GetUsername()
 * email -> GetEmail()
 * phoneNumber -> GetMobilePhoneNumber()
 * sessionToken -> GetSessionToken()
 * isAnonymous -> IsAnonymous()
 * avatar -> GetAvatar()
 * nickName -> GetNickName()
 * shortId -> GetShortID()
 * UpdateAt -> GetUpdatedAt()
 * CreateAt -> GetCreatedAt()
 * bMobilePhoneVerified -> IsMobilePhoneVerified()
 */

TAPBOOTSTRAP_API TSharedPtr<FJsonObject> GetJsonObject(const TSharedPtr<FLCObject>& ObjectPtr);
TAPBOOTSTRAP_API TSharedPtr<FLCObject> MakeLCObject(const TSharedPtr<FJsonObject>& ObjectPtr);
TAPBOOTSTRAP_API TSharedPtr<FLCObject> MakeLCObject(const TSharedPtr<FJsonObject>& ObjectPtr, const FString& InClassName);

class TAPBOOTSTRAP_API FTDSUser: public FLCUser
{
public:
	DECLARE_DELEGATE_TwoParams(FCallBackDelegate, const TSharedPtr<FTDSUser>& UserPtr, const FTUError& Error);
// #if PLATFORM_IOS
// 	FTDSUser(TDSUser *InUser) : FLCUser((LCUser *)InUser) {};
// #elif PLATFORM_ANDROID
// 	FTDSUser(jobject InUser) : FLCUser(InUser) {};
// 	FTDSUser(const TapJNI::Object& InUser) : FLCUser(*InUser) {};
// #endif
#if PLATFORM_IOS
	FTDSUser(const FTDSUser& OtherObject) : FLCUser(OtherObject) {}
	FTDSUser(FTDSUser&& OtherObject) : FLCUser(OtherObject) {}
#endif
	FString GetAvatar() const;
	FString GetNickName() const;
	FString GetShortID() const;
	void SetAvatar(const FString& InAvatar);
	void SetNickName(const FString& InNickName);
	void SetShortID(const FString& InShortID);
	
#if !PLATFORM_IOS && !PLATFORM_ANDROID
	void SetStringForKey(const FString& InKey, const FString& InValue);
	void SetDoubleForKey(const FString& InKey, double InValue);
	void SetIntergerForKey(const FString& InKey, int64 InValue);
	void SetBooleanForKey(const FString& InKey, bool InValue);
	FString GetStringFromKey(const FString& InKey) const;
	double GetDoubleFromKey(const FString& InKey) const;
	int64 GetIntergerFromKey(const FString& InKey) const;
	bool GetBooleanFromKey(const FString& InKey) const;

	TSharedPtr<FJsonObject> GetServerData() const;

#endif
	
	void Save(const FCallBackDelegate& CallBack);
	
	/**
	 * Return last login user from cached memory or load from local storage
	 */
	static TSharedPtr<FTDSUser> GetCurrentUser();

	/**
	 * Clear current user in memory and local storage
	 */
	static void Logout();

	/**
	 * Anonymously login to LeanCloud
	 * 
	 */
	// static void LoginAnonymously(const FString& AnonymouslyID, const FCallBackDelegate& CallBack);
	static void LoginAnonymously(const FCallBackDelegate& CallBack);

	/**
	 * Login LeanCloud with auth data (this auth data is TapTap),
	 * This evokes a TapTap login flow
	 * 
	 * @see LoginWithAuthData
	 * 
	 * @param Permissions @see TapUELogin::Login
	 */
	static void LoginWithTapTap(const TArray<FString>& Permissions, const FCallBackDelegate& CallBack);

	/**
	* Fetch LeanCloud user with LeanCloud's session token
	* 
	* @param SessionToken The session token pass-in
	*/
	static void BecomeWithSessionToken(const FString& SessionToken, const FCallBackDelegate& CallBack);

	/**
	 * Login LeanCloud with other platform account
	 * 
	 * @param Platform The other platform to auth with
	 * @param AuthData @see https://leancloud.cn/docs/rest_api.html#hash621643170
	 */
	static void LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, const FCallBackDelegate& CallBack);

	/**
	 * Bind other platform account to TapTap Developer Services user
	 * 
	 * @param Platform The other platform to auth with
	 * @param AuthData @see https://leancloud.cn/docs/rest_api.html#hash621643170
	 */
	void AssociateWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, const FCallBackDelegate& CallBack);

	/**
	 * Unbind other platform account to TapTap Developer Services user
	 * 
	 * @param Platform The other platform to unbind
	 */
	void DisassociateAuthData(const FString& Platform, const FCallBackDelegate& CallBack);
	
	static void RetrieveShortToken(const FStringSignature& OnSuccess, const FTUError::FDelegate& OnFailed);

	
protected:

	static FString KeyAvatar;
	static FString KeyNickName;
	static FString KeyShortID;

private:
	static void LoginWithUserCallBack(const FCallBackDelegate& CallBack,  FLeanCloudUserDelegate& LCCallBack);
	static void LoginWithBoolCallBack(const FCallBackDelegate& CallBack,  FLeanCloudBoolResultDelegate& LCCallBack, const TSharedPtr<FTDSUser>& InUserPtr);
};
