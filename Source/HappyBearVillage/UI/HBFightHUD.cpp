// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBFightHUD.h"

#include "HBDamagedPlayerHUD.h"

UHBFightHUD::UHBFightHUD(const FObjectInitializer& ObjectInitializer)
{
}

void UHBFightHUD::NativeConstruct()
{
	Super::NativeConstruct();

	FirstDamagedPlayerWidget = Cast<UHBDamagedPlayerHUD>(GetWidgetFromName(TEXT("DamagedPlayerFirst")));
	ensure(FirstDamagedPlayerWidget);
	
	SecondDamagedPlayerWidget = Cast<UHBDamagedPlayerHUD>(GetWidgetFromName(TEXT("DamagedPlayerSecond")));
	ensure(SecondDamagedPlayerWidget);
	
	ThirdDamagedPlayerWidget = Cast<UHBDamagedPlayerHUD>(GetWidgetFromName(TEXT("DamagedPlayerThird")));
	ensure(ThirdDamagedPlayerWidget);
}

void UHBFightHUD::UpdateCurrentInfo(AHBCharacterPlayer* InPlayer, int32 InRank)
{
	if (InRank == 1)
	{
		FirstDamagedPlayerWidget->UpdateCurrentInfo(InPlayer);
	}
	else if (InRank == 2)
	{
		SecondDamagedPlayerWidget->UpdateCurrentInfo(InPlayer);
	}
	else if (InRank == 3)
	{
		ThirdDamagedPlayerWidget->UpdateCurrentInfo(InPlayer);
	}
}
