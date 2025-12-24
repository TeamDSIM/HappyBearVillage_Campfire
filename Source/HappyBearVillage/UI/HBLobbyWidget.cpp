// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Controller/HBPlayerController.h"
#include "Engine/GameInstance.h"
#include "MultiplayerSessionsSubsystem.h"
#include "HBSteamFriendEntryWidget.h"
#include "GameFramework/PlayerState.h"

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

	//Subsystem이 친구 목록을 읽어왔을 때, 이 Widget의 OnFriendsReady가 호출되도록 구독
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnReadFriendsComplete.AddUObject(
			this, &ThisClass::OnFriendsReady
		);
	}

	// GameState 얻기
	CachedMafiaGS = GetWorld() ? GetWorld()->GetGameState<AHBMafiaGameState>() : nullptr;
	if (!CachedMafiaGS)
	{
		// GameState가 아직 없을 수 있으면(타이밍) 타이머로 재시도하는 방식도 가능
		return;
	}

	// Join/Leave 시 자동 갱신
	CachedMafiaGS->OnLobbyPlayersChanged.AddUObject(this, &ThisClass::HandleLobbyPlayersChanged);

	// 처음 1회 즉시 갱신
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

//친구목록 갱신 요청
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

	//친구 목록 갱신
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

	// 친구 목록 정렬
	TArray<FHBSteamFriend> SortedFriends = Friends;

	SortedFriends.Sort([](const FHBSteamFriend& A, const FHBSteamFriend& B)
		{
			// 1순위: Online 먼저
			if (A.bIsOnline != B.bIsOnline)
			{
				return A.bIsOnline > B.bIsOnline; // true가 앞으로
			}

			// 2순위(선택): 이름 오름차순
			return A.DisplayName < B.DisplayName;
		});


	//기존 친구 목록 clear
	ScrollBox_Friends->ClearChildren();

	// 1. 친구 수만큼 Entry Widget 생성해서 ScrollBox에 추가
	for (const FHBSteamFriend& F : SortedFriends)
	{
		UHBSteamFriendEntryWidget* Entry =
			CreateWidget<UHBSteamFriendEntryWidget>(GetOwningPlayer(), FriendEntryWidgetClass);

		if (!Entry) continue;

		// 2. Entry Widgt에 표시할 데이터 세팅
		Entry->Init(F.DisplayName, F.NetIdStr, F.bIsOnline);

		// 3. 엔트리의 “초대” 클릭 → LobbyWidget으로 전달
		Entry->OnInviteClicked.AddUObject(this, &ThisClass::HandleInviteClicked);

		// 4. ScrollBox에 추가
		ScrollBox_Friends->AddChild(Entry);
	}
}

//친구 초대 
void UHBLobbyWidget::HandleInviteClicked(const FString& NetIdStr)
{
	// 엔트리 위젯에서 "초대 버튼"을 누르면 여기로 들어옴
// UI는 Subsystem에게 "이 NetId 친구 초대해"라고 전달만 한다.

	if (!MultiplayerSessionsSubsystem)
	{
		return;
	}

	MultiplayerSessionsSubsystem->InviteFriendByNetIdStr(NetIdStr);
}

// 로비의 플레이어 목록 갱신, 표시
void UHBLobbyWidget::RefreshPlayersInRoom()
{
	if (!CachedMafiaGS || !ScrollBox_LobbyPlayers) return;

	ScrollBox_LobbyPlayers->ClearChildren();

	for (APlayerState* PS : CachedMafiaGS->PlayerArray)
	{
		if (!PS) continue;
		// TextBlock 하나로 이름 표시
		UTextBlock* NameText = NewObject<UTextBlock>(this);
		// 플레이어 목록 받아오기
		NameText->SetText(FText::FromString(PS->GetPlayerName()));
		ScrollBox_LobbyPlayers->AddChild(NameText);
	}


	//if (!ScrollBox_LobbyPlayers) return;

	//ScrollBox_LobbyPlayers->ClearChildren();

	//// 방에 있는 모든 PlayerState 순회
	//AGameStateBase* GS = GetWorld()->GetGameState();
	//if (!GS) return;

	//for (APlayerState* PS : GS->PlayerArray)
	//{
	//	if (!PS) continue;

	//	// TextBlock 하나로 이름 표시
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
