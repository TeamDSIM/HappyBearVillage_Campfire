// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBCharacterRole.h"
#include "HBCharacterColor.generated.h"

/**
 *	작성일: 2025-01-07
 *  작성자: 박현용
 *  설명: 캐릭터의 색상 관련 열거형 및 구조체 생성
 */

UENUM(BlueprintType)
enum class EColorType : uint8
{
	// 색상 목록
	RED UMETA(DisplayName = "Red"),
	BLUE UMETA(DisplayName = "Blue"),
	GREEN UMETA(DisplayName = "Green"),
	YELLOW UMETA(DisplayName = "Yellow"),
	CYAN UMETA(DisplayName = "Cyan"),
	WHITE UMETA(DisplayName = "White"),
	PINK UMETA(DisplayName = "Pink"),
	ORANGE UMETA(DisplayName = "Orange"),

	END UMETA(DisplayName = "End") // 총 개수
};

USTRUCT(BlueprintType)
struct FHBCharacterColor
{
	GENERATED_BODY()

public:
	FHBCharacterColor()
		: Color(EColorType::BLUE), ColorValue(FLinearColor::Blue)
	{
	}

	void SetColor(EColorType InColor)
	{
		Color = InColor;
		
		switch (InColor)
		{
		case EColorType::RED:
			ColorValue = FLinearColor::Red;
			break;
		case EColorType::BLUE:
			ColorValue = FLinearColor::Blue;
			break;
		case EColorType::GREEN:
			ColorValue = FLinearColor::Green;
			break;
		case EColorType::YELLOW:
			ColorValue = FLinearColor::Yellow;
			break;
		case EColorType::CYAN:
			ColorValue.R = 0.f; ColorValue.G = 1.f; ColorValue.B = 1.f; ColorValue.A = 1.f;
			break;
		case EColorType::WHITE:
			ColorValue = FLinearColor::White;
			break;
		case EColorType::PINK:
			ColorValue.R = 1.f; ColorValue.G = 0.f; ColorValue.B = 0.8f; ColorValue.A = 1.f;
			break;
		case EColorType::ORANGE:
			ColorValue.R = 1.f; ColorValue.G = 0.6f; ColorValue.B = 0.f; ColorValue.A = 1.f;
			break;
		default:
			break;
		}	
	}
	
	EColorType GetColor() const { return Color; }
	FLinearColor GetColorValue() const { return ColorValue; }
	
	// 마피아 / 시민 종류 열거형
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
	EColorType Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
	FLinearColor ColorValue;

	

	
};
