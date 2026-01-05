// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBFadeHUD.h"

#include "HappyBearVillage.h"

UHBFadeHUD::UHBFadeHUD()
{
}

void UHBFadeHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UHBFadeHUD::PlayFadeIn()
{
	UE_LOG(LogTemp, Log, TEXT("PlayFadeIn"));
	StopAllAnimations();
	PlayAnimation(FadeIn, 0.f, 1);
}

void UHBFadeHUD::PlayFadeOut()
{
	UE_LOG(LogTemp, Log, TEXT("PlayFadeOut"));
	StopAllAnimations();
	PlayAnimation(FadeOut, 0.f, 1);
}
