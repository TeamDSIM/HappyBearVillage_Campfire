// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBVoteHUD.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBVoteHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBVoteHUD();

public:
	void NativeConstruct() override;

public:
	void UpdateVoteNum(int32 NewVoteNum);

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> VoteNumText;
};
