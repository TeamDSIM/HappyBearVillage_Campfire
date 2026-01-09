// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBCreditWidget.h"

void UHBCreditWidget::PlayCredits()
{
	UE_LOG(LogTemp, Log, TEXT("PlayCredits"));
	//0초부터 1회 재생
	PlayAnimation(CreditScrollAnim, 0.f, 1);
}

void UHBCreditWidget::CloseCredits()
{
}

