// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBFightHUD.h"

#include "HBDamagedPlayerHUD.h"
#include "GameState/HBMafiaGameState.h"

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

void UHBFightHUD::UpdateCurrentInfo(AHBPlayerState* InPlayer, FDamageRankEntry InEntry, int32 InRank)
{
	if (InRank == 0)
	{
		FirstDamagedPlayerWidget->UpdateCurrentInfo(InPlayer, InEntry);
	}
	else if (InRank == 1)
	{
		SecondDamagedPlayerWidget->UpdateCurrentInfo(InPlayer, InEntry);
	}
	else if (InRank == 2)
	{
		ThirdDamagedPlayerWidget->UpdateCurrentInfo(InPlayer, InEntry);
	}
}
