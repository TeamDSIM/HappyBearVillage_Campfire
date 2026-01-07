// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBEffectToolTip.h"

#include "Components/TextBlock.h"

UHBEffectToolTip::UHBEffectToolTip(const FObjectInitializer& ObjectInitializer)
{
}

void UHBEffectToolTip::SetToolTipText(const FText& InText)
{
	if (Description)
	{
		Description->SetText(InText);
	}
}

void UHBEffectToolTip::NativeConstruct()
{
	Super::NativeConstruct();
}
