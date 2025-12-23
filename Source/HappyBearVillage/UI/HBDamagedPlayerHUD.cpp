// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBDamagedPlayerHUD.h"

#include "AssetSelection.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"

UHBDamagedPlayerHUD::UHBDamagedPlayerHUD(const FObjectInitializer& ObjectInitializer)
{
}

void UHBDamagedPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	RankImage = Cast<UImage>(GetWidgetFromName(TEXT("Rank")));
	ensure(RankImage);

	PlayerColorImage = Cast<UImage>(GetWidgetFromName(TEXT("PlayerColor")));
	ensure(PlayerColorImage);

	PlayerNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
	ensure(PlayerNameText);

	PlayerTotalDamagedText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalDamaged")));
	ensure(PlayerTotalDamagedText);

}

void UHBDamagedPlayerHUD::UpdateCurrentInfo(AHBPlayerState* InPlayer, FDamageRankEntry InEntry)
{
	UpdatePlayerColor(InPlayer->GetPlayerColor());
	UpdatePlayerName(InPlayer->GetName());
	UpdateTotalDamaged(InEntry.TotalTakenDamaged);
}

void UHBDamagedPlayerHUD::UpdatePlayerColor(FLinearColor NewColor)
{
	if (PlayerColorImage)
	{
		UE_LOG(LogTemp, Log, TEXT("[UHBDamagedPlayerHUD] UpdatePlayerColor"));
		PlayerColorImage->SetBrushTintColor(NewColor);
	}
}

void UHBDamagedPlayerHUD::UpdatePlayerName(FString NewName)
{
	PlayerNameString = NewName;
	if (PlayerNameText)
	{
		UE_LOG(LogTemp, Log, TEXT("[UHBDamagedPlayerHUD] UpdatePlayerName : %s"), *NewName);
		PlayerNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *PlayerNameString)));
	}
}

void UHBDamagedPlayerHUD::UpdateTotalDamaged(float NewTotalDamaged)
{
	if (PlayerTotalDamagedText)
	{
		UE_LOG(LogTemp, Log, TEXT("[UHBDamagedPlayerHUD] UpdateTotalDamaged : %f"), NewTotalDamaged);
		PlayerTotalDamagedText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewTotalDamaged)));
	}
}
