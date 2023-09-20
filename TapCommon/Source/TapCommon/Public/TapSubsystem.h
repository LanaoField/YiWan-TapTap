// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapSubsystem.generated.h"

class SWidget;
class SDPIScaler;
class SOverlay;
class UUserInterfaceSettings;
/**
 * 
 */
UCLASS(Config = "Game", DefaultConfig)
class TAPCOMMON_API UTapSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	static UTapSubsystem* Get();

	static void AddWidget(TSharedRef<SWidget> InWidget, int32 ZOrder = 0);

	static void RemoveWidget(TSharedRef<SWidget> InWidget);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetupTapWidgetScaler();
	
	virtual void Deinitialize() override;

	float CalculateTapDPI() const;
	
	TSharedPtr<SDPIScaler> Scaler;
	TSharedPtr<SOverlay> Overlay;
	
	UPROPERTY()
	UUserInterfaceSettings* UISetting;

	UPROPERTY(Config, EditAnywhere)
	int32 TapWidgetZOrder = 9999;
};

