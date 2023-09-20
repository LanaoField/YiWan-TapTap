#pragma once

class LEANCLOUDMOBILE_API LCHelper {
public:
	static void PerformOnGameThread(TFunction<void()> Function);

};
