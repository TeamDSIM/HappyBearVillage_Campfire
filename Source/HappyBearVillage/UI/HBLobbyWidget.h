// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HBSteamFriendEntryWidget.h"
#include "HBLobbyWidget.generated.h"

/**
 * 
 */

class UMultiplayerSessionsSubsystem;
class UButton;

struct FHBSteamFriend;

UCLASS()
class HAPPYBEARVILLAGE_API UHBLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetFriendInviteVisible(bool bVisible);

private:

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void ExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Friends;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LobbyScrollBoxTitle;


	// Subsystem 캐시
	UPROPERTY()
	TObjectPtr<UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	//“친구 한 줄” 위젯 BP 클래스 (WBP_FriendEntry)
	UPROPERTY(EditDefaultsOnly, Category = "Friends")
	TSubclassOf<UHBSteamFriendEntryWidget> FriendEntryWidgetClass;

	// 친구 목록 갱신 요청
	void RequestFriends();

	// Subsystem에서 친구목록 읽기 완료 시 호출될 함수
	void OnFriendsReady(const TArray<FHBSteamFriend>& Friends, bool bWasSuccessful);

	// FriendEntry에서 “초대” 눌렀을 때 처리
	void HandleInviteClicked(const FString& NetIdStr);

};
