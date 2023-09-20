// Fill out your copyright notice in the Description page of Project Settings.


#include "TapWidget.h"

#include "TapSubsystem.h"

void UTapWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
	if ( UPanelWidget* ParentPanel = GetParent() )
	{
		return;
	}

	TSharedRef<SWidget> UserSlateWidget = TakeWidget();

	UTapSubsystem::AddWidget(UserSlateWidget);
	
	// Just in case we already hooked this delegate, remove the handler.
	FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);

	// Widgets added to the viewport are automatically removed if the persistent level is unloaded.
	FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UTapWidget::OnLevelRemovedFromWorld);
}

void UTapWidget::RemoveFromParent()
{
	if (!HasAnyFlags(RF_BeginDestroyed))
	{
		if (GetCachedWidget().IsValid())
		{
			UTapSubsystem::RemoveWidget(GetCachedWidget().ToSharedRef());
			
			FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
		}
		else
		{
			UWidget::RemoveFromParent();
		}
	}
}
