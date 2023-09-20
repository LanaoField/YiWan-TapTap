// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TapWidget.generated.h"


UCLASS()
class TAPCOMMON_API UTapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

	virtual void RemoveFromParent() override;
};
