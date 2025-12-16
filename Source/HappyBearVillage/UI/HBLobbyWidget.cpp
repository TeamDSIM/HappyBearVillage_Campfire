// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Engine/GameInstance.h"
#include "../Plugins/MultiPlayerSessions//Source/MultiPlayerSessions/Public/MultiplayerSessionsSubsystem.h"

//화면에 나타날 준비가 모두 끝난 경우
void UHBLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameInstance* GI = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		//MultiCast Delegate 바인딩
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUObject(
			this, &ThisClass::OnCreateSessionComplete);
	}

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
	UE_LOG(LogTemp, Log, TEXT("Host Made the Room"));

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(4, TEXT("FreeForAll"));
	}
}

void UHBLobbyWidget::JoinButtonClicked()
{

	UE_LOG(LogTemp, Log, TEXT("Client Joined the Room"));


}

void UHBLobbyWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("CreateSessionComplete: %s"), bWasSuccessful ? TEXT("Success") : TEXT("Fail"));
}
