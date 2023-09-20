#include "Android/TapBootstrapJavaHelper.h"
#include "TapJavaHelper.h"

TSharedPtr<FTDSUser> TapBootstrapJavaHelper::ConvertUser(const TapJNI::Object& user) {
	if (!user) {
		return nullptr;
	}
	TSharedPtr<FLCUser> lcUser = MakeShared<FLCUser>(*user);
	return StaticCastSharedPtr<FTDSUser>(lcUser);
}
