// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum class ELanguageType : uint8;

typedef TSharedPtr<class FTapAchievementCommon, ESPMode::ThreadSafe> FTapAchievementsPtr;

class TAPACHIEVEMENT_API FTapAchievementModule : public IModuleInterface
{
public:
	/**
	 * Get tap achievement interface
	 * @Return Tap achievement interface singleton
	 */
	static FTapAchievementsPtr GetAchievementInterface();

protected:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;

	void OnLanguageChanged(ELanguageType OldType, ELanguageType NewType);

	FTapAchievementsPtr TapAchievement;
};

