#include "LCJNICallbackHelper.h"


TMap<int, TSharedPtr<FLCJNICallbackHelper::FModel>> FLCJNICallbackHelper::CallBackModels;
int FLCJNICallbackHelper::CallBackID = 0;