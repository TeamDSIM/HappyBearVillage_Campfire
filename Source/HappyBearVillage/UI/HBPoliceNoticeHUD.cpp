// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBPoliceNoticeHUD.h"

#include "HBEffectToolTip.h"
#include "Components/TextBlock.h"

UHBPoliceNoticeHUD::UHBPoliceNoticeHUD(const FObjectInitializer& ObjectInitializer)
{
	
}

void UHBPoliceNoticeHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SetEffectToolTipWidget();
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UHBPoliceNoticeHUD::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Log, TEXT("[HBPoliceNoticeHUD] NativeOnMouseEnter"));
}

void UHBPoliceNoticeHUD::SetEffectToolTipWidget()
{
	if (!EffectToolTipClass)
	{
		return;
	}

	EffectToolTipWidget = CreateWidget<UHBEffectToolTip>(this, EffectToolTipClass);
	
	if (EffectToolTipWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("[HBPoliceNoticeHUD] SetEffectToolTipWidget"));
		//EffectToolTipWidget->SetToolTipText(FText::FromString("경찰이 집을 방문했습니다. 스킬 사용/무기 장착이 불가합니다."));
		EffectToolTipWidget->SetToolTipText(FText::FromString("The police have arrived. Skills and weapons are disabled."));
		SetToolTip(EffectToolTipWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[HBPoliceNoticeHUD] EffectToolTipWidget is NULL"));
	}
}
