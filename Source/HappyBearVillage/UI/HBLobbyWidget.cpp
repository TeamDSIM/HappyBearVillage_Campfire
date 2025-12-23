// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Controller/HBPlayerController.h"
#include "Engine/GameInstance.h"
#include "MultiplayerSessionsSubsystem.h"
#include "HBSteamFriendEntryWidget.h"

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

	//Subsystem 가져오기, Delegate 연결
	if (UGameInstance* GI = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnReadFriendsComplete.AddUObject(
			this, &ThisClass::OnFriendsReady
		);
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

void UHBLobbyWidget::SetFriendInviteVisible(bool bVisible)
{
	if (!ScrollBox_Friends || !LobbyScrollBoxTitle)
	{
		return;
	}

	ScrollBox_Friends->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	LobbyScrollBoxTitle->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (bVisible)
	{
		RequestFriends();
	}

}


void UHBLobbyWidget::RequestFriends()
{
	if (!MultiplayerSessionsSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestFriends: Subsystem is null"));
		return;
	}

	// FriendEntryWidgetClass가 비어있으면 UI 생성 못함
	if (!FriendEntryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestFriends: FriendEntryWidgetClass is null (set in BP Class Defaults)"));
		return;
	}

	MultiplayerSessionsSubsystem->ReadFriendsList();
}

void UHBLobbyWidget::OnFriendsReady(const TArray<FHBSteamFriend>& Friends, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFriendsReady: failed"));
		return;
	}

	if (!ScrollBox_Friends || !FriendEntryWidgetClass)
	{
		return;
	}

	ScrollBox_Friends->ClearChildren();

	for (const FHBSteamFriend& F : Friends)
	{
		UHBSteamFriendEntryWidget* Entry =
			CreateWidget<UHBSteamFriendEntryWidget>(GetOwningPlayer(), FriendEntryWidgetClass);

		if (!Entry) continue;

		Entry->Init(F.DisplayName, F.NetIdStr, F.bIsOnline);

		// 엔트리의 “초대” 클릭 → LobbyWidget으로 전달
		Entry->OnInviteClicked.AddUObject(this, &ThisClass::HandleInviteClicked);

		ScrollBox_Friends->AddChild(Entry);
	}
}

void UHBLobbyWidget::HandleInviteClicked(const FString& NetIdStr)
{
	if (!MultiplayerSessionsSubsystem)
	{
		return;
	}

	MultiplayerSessionsSubsystem->InviteFriendByNetIdStr(NetIdStr);
}
