// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBGameEndHUD.h"

#include "Components/TextBlock.h"

UHBGameEndHUD::UHBGameEndHUD()
{
}

void UHBGameEndHUD::NativeConstruct()
{
	Super::NativeConstruct();

	WinText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Win")));
	ensure(WinText);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UHBGameEndHUD::UpdateWinText(FString NewWinText)
{
	WinTextString = NewWinText;
	if (WinText)
	{
		WinText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *WinTextString)));
	}
}
