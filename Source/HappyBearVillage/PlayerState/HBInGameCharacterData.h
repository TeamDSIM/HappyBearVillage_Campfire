// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Stat/HBCharacterRole.h"
#include "HBInGameCharacterData.generated.h"

USTRUCT(BlueprintType)
struct FHBInGameCharacterData
{
	GENERATED_BODY()

public:
	FHBInGameCharacterData()
		: PlayerColor(FLinearColor::Gray)
	{}

	void SetPlayerColor(FLinearColor NewColor)
	{
		PlayerColor = NewColor;
	}
	
	void SetCharacterRole(EJobType InJob)
	{
		CharacterRole.InitRole(InJob);
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InGameCharacterData)
	FLinearColor PlayerColor = FLinearColor::Gray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InGameCharacterData)
	FHBCharacterRole CharacterRole;
};