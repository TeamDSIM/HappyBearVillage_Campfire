// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Engine/GameInstance.h"

//화면에 나타날 준비가 모두 끝난 경우
void UHBLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitButtonClicked);
	}

}

void UHBLobbyWidget::StartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("StartButtonClicked"));
}

void UHBLobbyWidget::ExitButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ExitButtonClicked"));
}
