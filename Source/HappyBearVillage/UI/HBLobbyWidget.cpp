// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"


//화면에 나타날 준비가 모두 끝난 경우
void UHBLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

}

void UHBLobbyWidget::HostButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("HostButtonClicked"));
}

void UHBLobbyWidget::JoinButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("JoinButtonClicked"));
}
