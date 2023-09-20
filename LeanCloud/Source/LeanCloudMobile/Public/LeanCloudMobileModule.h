// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LeanCloudMobile.h"
#include "Modules/ModuleManager.h"

class FLeanCloudMobileModule : public IModuleInterface
{
public:
protected:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
