// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBVoteHUD.h"

#include "Components/TextBlock.h"

UHBVoteHUD::UHBVoteHUD()
{
}

void UHBVoteHUD::NativeConstruct()
{
	Super::NativeConstruct();

	VoteNumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("VoteNum")));
	ensure(VoteNumText);
}

void UHBVoteHUD::UpdateVoteNum(int32 NewVoteNum)
{
	if (VoteNumText)
	{
		VoteNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewVoteNum)));
	}
}
