#include "IOS/TapBootstrapObjcHelper.h"
#include "iOS/LCObjcHelper.h"
#include "IOSHelper.h"


TSharedPtr<FTDSUser> TapBootstrapObjcHelper::Convert(TDSUser *user) {
	TSharedPtr<FLCUser> lcUserPtr = LCObjcHelper::Convert((LCUser *)user);
	if (lcUserPtr.IsValid()) {
		return StaticCastSharedPtr<FTDSUser>(lcUserPtr);
	} else {
		return nullptr;
	}
}
