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

	//Subsystem이 친구 목록을 읽어왔을 때, 이 Widget의 OnFriendsReady가 호출되도록 구독
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

	//기존 친구 목록 clear
	ScrollBox_Friends->ClearChildren();

	// 1. 친구 수만큼 Entry Widget 생성해서 ScrollBox에 추가
	for (const FHBSteamFriend& F : Friends)
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
