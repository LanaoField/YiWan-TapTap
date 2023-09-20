#pragma once

#include "CoreMinimal.h"

class TAPCOMMON_API IBridge
{

public:
    virtual void Init() = 0;
    
    virtual void Register(FString serviceClz, FString serviceImpl) = 0;
    
    virtual void CallHandler(FString command) = 0;
};

IBridge *GetBridge();
