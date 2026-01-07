// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Controller/HBPlayerController.h"
#include "Engine/GameInstance.h"
#include "MultiplayerSessionsSubsystem.h"
#include "HBSteamFriendEntryWidget.h"
#include "Controller/HBLobbyPlayerController.h"
#include "GameFramework/PlayerState.h"

//ȭ�鿡 ��Ÿ�� �غ� ��� ���� ���
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

	//Subsystem ��������, Delegate ����
	if (UGameInstance* GI = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	//Subsystem�� ģ�� ����� �о���� ��, �� Widget�� OnFriendsReady�� ȣ��ǵ��� ����
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnReadFriendsComplete.AddUObject(
			this, &ThisClass::OnFriendsReady
		);
	}

	// GameState ���
	CachedMafiaGS = GetWorld() ? GetWorld()->GetGameState<AHBMafiaGameState>() : nullptr;
	if (!CachedMafiaGS)
	{
		// GameState�� ���� ���� �� ������(Ÿ�̹�) Ÿ�̸ӷ� ��õ��ϴ� ��ĵ� ����
		return;
	}

	// Join/Leave �� �ڵ� ����
	CachedMafiaGS->OnLobbyPlayersChanged.AddUObject(this, &ThisClass::HandleLobbyPlayersChanged);

	// ó�� 1ȸ ��� ����
	RefreshPlayersInRoom();
}

void UHBLobbyWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (CachedMafiaGS)
	{
		CachedMafiaGS->OnLobbyPlayersChanged.RemoveAll(this);
	}
}


void UHBLobbyWidget::StartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("StartButtonClicked"));
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AHBLobbyPlayerController* HBPC = Cast<AHBLobbyPlayerController>(PC))
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
		if (AHBLobbyPlayerController* HBPC = Cast<AHBLobbyPlayerController>(PC))
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

//ģ����� ���� ��û
void UHBLobbyWidget::RequestFriends()
{
	
	if (!MultiplayerSessionsSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestFriends: Subsystem is null"));
		return;
	}

	// FriendEntryWidgetClass�� ��������� UI ���� ����
	if (!FriendEntryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("RequestFriends: FriendEntryWidgetClass is null (set in BP Class Defaults)"));
		return;
	}

	//ģ�� ��� ����
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

	// ģ�� ��� ����
	TArray<FHBSteamFriend> SortedFriends = Friends;

	SortedFriends.Sort([](const FHBSteamFriend& A, const FHBSteamFriend& B)
		{
			// 1����: Online ����
			if (A.bIsOnline != B.bIsOnline)
			{
				return A.bIsOnline > B.bIsOnline; // true�� ������
			}

			// 2����(����): �̸� ��������
			return A.DisplayName < B.DisplayName;
		});


	//���� ģ�� ��� clear
	ScrollBox_Friends->ClearChildren();

	// 1. ģ�� ����ŭ Entry Widget �����ؼ� ScrollBox�� �߰�
	for (const FHBSteamFriend& F : SortedFriends)
	{
		UHBSteamFriendEntryWidget* Entry =
			CreateWidget<UHBSteamFriendEntryWidget>(GetOwningPlayer(), FriendEntryWidgetClass);

		if (!Entry) continue;

		// 2. Entry Widgt�� ǥ���� ������ ����
		Entry->Init(F.DisplayName, F.NetIdStr, F.bIsOnline);

		// 3. ��Ʈ���� ���ʴ롱 Ŭ�� �� LobbyWidget���� ����
		Entry->OnInviteClicked.AddUObject(this, &ThisClass::HandleInviteClicked);

		// 4. ScrollBox�� �߰�
		ScrollBox_Friends->AddChild(Entry);
	}
}

//ģ�� �ʴ� 
void UHBLobbyWidget::HandleInviteClicked(const FString& NetIdStr)
{
	// ��Ʈ�� �������� "�ʴ� ��ư"�� ������ ����� ����
// UI�� Subsystem���� "�� NetId ģ�� �ʴ���"��� ���޸� �Ѵ�.

	if (!MultiplayerSessionsSubsystem)
	{
		return;
	}

	MultiplayerSessionsSubsystem->InviteFriendByNetIdStr(NetIdStr);
}

// �κ��� �÷��̾� ��� ����, ǥ��
void UHBLobbyWidget::RefreshPlayersInRoom()
{
	if (!CachedMafiaGS || !ScrollBox_LobbyPlayers) return;

	ScrollBox_LobbyPlayers->ClearChildren();

	for (APlayerState* PS : CachedMafiaGS->PlayerArray)
	{
		if (!PS) continue;
		// TextBlock �ϳ��� �̸� ǥ��
		UTextBlock* NameText = NewObject<UTextBlock>(this);
		// �÷��̾� ��� �޾ƿ���
		NameText->SetText(FText::FromString(PS->GetPlayerName()));
		ScrollBox_LobbyPlayers->AddChild(NameText);
	}


	//if (!ScrollBox_LobbyPlayers) return;

	//ScrollBox_LobbyPlayers->ClearChildren();

	//// �濡 �ִ� ��� PlayerState ��ȸ
	//AGameStateBase* GS = GetWorld()->GetGameState();
	//if (!GS) return;

	//for (APlayerState* PS : GS->PlayerArray)
	//{
	//	if (!PS) continue;

	//	// TextBlock �ϳ��� �̸� ǥ��
	//	UTextBlock* NameText = NewObject<UTextBlock>(this);
	//	NameText->SetText(FText::FromString(PS->GetPlayerName()));

	//	ScrollBox_LobbyPlayers->AddChild(NameText);
	//}
}

void UHBLobbyWidget::HandleLobbyPlayersChanged()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RefreshPlayersInRoom);
}
