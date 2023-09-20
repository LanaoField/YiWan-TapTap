#pragma once
#include "TapJNI.h"
#include "TDSUser.h"

class TAPBOOTSTRAP_API TapBootstrapJavaHelper {
public:
	static TSharedPtr<FTDSUser> ConvertUser(const TapJNI::Object& tds_user);
};
