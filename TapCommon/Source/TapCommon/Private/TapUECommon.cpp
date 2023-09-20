#include "TapUECommon.h"

#include "TUCommonImpl.h"

void TapUECommon::SetLanguage(ELanguageType LanguageType) {
	TUCommonImpl::Get()->SetLanguage(LanguageType);
}

void TapUECommon::setDurationStatisticsEnabled(bool bEnable) {
	TUCommonImpl::Get()->setDurationStatisticsEnabled(bEnable);
}
