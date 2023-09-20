#pragma once
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <TapBootstrapSDK/TapBootstrapSDK.h>

#include "TDSUser.h"

class TAPBOOTSTRAP_API TapBootstrapObjcHelper {
public:
	static TSharedPtr<FTDSUser> Convert(TDSUser *user);

};
