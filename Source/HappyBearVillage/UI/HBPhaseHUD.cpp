// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBPhaseHUD.h"

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

}
