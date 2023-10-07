// Fill out your copyright notice in the Description page of Project Settings.


#include "TapSubsystem.h"

#include "TUSettings.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/UserInterfaceSettings.h"
#include "Widgets/Layout/SDPIScaler.h"

UTapSubsystem* UTapSubsystem::Get()
{
	if (UGameInstance* GI = TUSettings::GetGameInstance().Get())
	{
		if (ULocalPlayer* Player = GI->GetFirstGamePlayer())
		{
			return Cast<UTapSubsystem>(Player->GetSubsystemBase(UTapSubsystem::StaticClass()));
		}
	}
	return nullptr;
}

void UTapSubsystem::AddWidget(TSharedRef<SWidget> InWidget, int32 ZOrder)
{
	if (UTapSubsystem* Sys = UTapSubsystem::Get())
	{
		if (Sys->Overlay)
		{
			Sys->Overlay->AddSlot(ZOrder)
			[
				InWidget
			];
		}
	}
}

void UTapSubsystem::RemoveWidget(TSharedRef<SWidget> InWidget)
{
	if (UTapSubsystem* Sys = UTapSubsystem::Get())
	{
		if (Sys->Overlay)
		{
			Sys->Overlay->RemoveSlot(InWidget);
		}
	}
}

void UTapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameViewportClient::OnViewportCreated().AddUObject(this, &UTapSubsystem::SetupTapWidgetScaler);
}

void UTapSubsystem::SetupTapWidgetScaler()
{
	UGameViewportClient::OnViewportCreated().RemoveAll(this);
	
	check(GEngine && GEngine->GameViewport);

	UISetting = NewObject<UUserInterfaceSettings>();
	UISetting->ApplicationScale = 1.f;
	UISetting->UIScaleRule = EUIScalingRule::ShortestSide;
	if (FProperty* Pro = UISetting->GetClass()->FindPropertyByName(TEXT("UIScaleCurve")))
	{
		Pro->ImportText(TEXT("(EditorCurveData=(Keys=((Time=480,Value=0.444),(Time=720,Value=0.666),(Time=1080,Value=1.0),(Time=8640,Value=8.0))),ExternalCurve=None)"), &UISetting->UIScaleCurve, 0, UISetting);
	}
	else
	{
		ensureMsgf(false, TEXT("Init tap dpi scale rule curve failed."));
	}
	
	SAssignNew(Scaler, SDPIScaler)
		.DPIScale_UObject(this, &UTapSubsystem::CalculateTapDPI)
	[
		SAssignNew(Overlay, SOverlay)
	];

	GEngine->GameViewport->AddViewportWidgetContent(Scaler.ToSharedRef(), TapWidgetZOrder);
}

void UTapSubsystem::Deinitialize()
{
	if (Scaler.IsValid() && GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(Scaler.ToSharedRef());
	}
	Overlay.Reset();
	Scaler.Reset();
}

float UTapSubsystem::CalculateTapDPI() const
{
	float GameScale = 1.0f;

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport);
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	float DefaultScale = UISetting->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
	
	return GameScale > 0.f ? DefaultScale / GameScale : 1.f;
}
