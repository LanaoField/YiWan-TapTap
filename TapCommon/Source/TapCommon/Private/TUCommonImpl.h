#pragma once
#include "TUType.h"

class TUCommonImpl {
public:

	virtual ~TUCommonImpl() = default;

	static TSharedPtr<TUCommonImpl>& Get();
	
	virtual void SetLanguage(ELanguageType LanguageType);

	virtual void setDurationStatisticsEnabled(bool bEnable);

private:
	static TSharedPtr<TUCommonImpl> Instance;
	
};
