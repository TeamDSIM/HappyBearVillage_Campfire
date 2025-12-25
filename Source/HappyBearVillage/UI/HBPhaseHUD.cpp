// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBPhaseHUD.h"

#include "Interface/HBCharacterHUDInterface.h"

UHBPhaseHUD::UHBPhaseHUD(const FObjectInitializer& ObjectInitializer)
{
}

void UHBPhaseHUD::NativeConstruct()
{
	Super::NativeConstruct();

	PhaseText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Phase")));
	ensure(PhaseText);

	TimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Time")));
	ensure(TimeText);
	
	DateText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Date")));
	ensure(DateText);
	
}

void UHBPhaseHUD::UpdateCurrentPhase(EGamePhase NewPhase)
{
	const UEnum* Enum = StaticEnum<EGamePhase>();
	if (!Enum)
	{
		UE_LOG(LogTemp, Error, TEXT("EGamePhase enum is null"));
		return;
	}
	
	PhaseString = Enum->GetNameStringByValue(static_cast<int32>(NewPhase));

	if (PhaseText)
	{
		PhaseText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *PhaseString)));
	}
}

void UHBPhaseHUD::UpdateCurrentTime(float NewTime)
{
	if (TimeText)
	{
		TimeText->SetText(FText::FromString(FString::Printf(TEXT("%d : %d"),
			((int32)NewTime / 60), ((int32)NewTime % 60))));
	}
}

void UHBPhaseHUD::UpdateCurrentDate(int32 NewDate)
{
	if (DateText)
	{
		DateText->SetText(FText::FromString(FString::Printf(TEXT("Day %d"),
			NewDate)));
	}
}
