// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBFightHUD.h"

#include "HBDamagedPlayerHUD.h"
#include "Components/Image.h"
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

	UImage* FirstImage = FirstDamagedPlayerWidget->GetRankImage();
	if (FirstImage)
	{
		FirstImage->SetBrushFromTexture(FirstMedalTexture, true);
		FirstImage->SetDesiredSizeOverride(FVector2D(64.f, 64.f));
	}
	UImage* SecondImage = SecondDamagedPlayerWidget->GetRankImage();
	if (SecondImage)
	{
		SecondImage->SetBrushFromTexture(SecondMedalTexture, true);
		SecondImage->SetDesiredSizeOverride(FVector2D(64.f, 64.f));
	}
	UImage* ThirdImage = ThirdDamagedPlayerWidget->GetRankImage();
	if (ThirdImage)
	{
		ThirdImage->SetBrushFromTexture(ThirdMedalTexture, true);
		ThirdImage->SetDesiredSizeOverride(FVector2D(64.f, 64.f));
	}
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
