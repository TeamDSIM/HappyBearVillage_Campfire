// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Controller/HBPlayerController.h"
#include "Engine/GameInstance.h"


//화면에 나타날 준비가 모두 끝난 경우
void UHBLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (StartButton)
	{
		//StartButton->IsFocusable = false;
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
	}

	if (ExitButton)
	{
		//ExitButton->IsFocusable = false;
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitButtonClicked);
	}

}

void UHBLobbyWidget::StartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("StartButtonClicked"));
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AHBPlayerController* HBPC = Cast<AHBPlayerController>(PC))
		{
			HBPC->StartGame();
		}
	}

}

void UHBLobbyWidget::ExitButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ExitButtonClicked"));
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AHBPlayerController* HBPC = Cast<AHBPlayerController>(PC))
		{
			HBPC->ExitGame();
		}
	}
}
