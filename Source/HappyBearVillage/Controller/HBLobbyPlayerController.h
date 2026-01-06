// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HBLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API AHBLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHBLobbyPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ResetUI();
	void SetupUI();
	void RemoveUI();

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ExitGame();

protected:
	void CreateLobbyUI();
	void ToggleFriendInvite();

	void HandleExitDestroySessionComplete(bool bWasSuccessful);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> PlayerIMC;
	
	//키 입력
	// IMC는 이미 Character에서 추가해서 필요 X
	// BP에서 지정 
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> FriendInviteInputAction;

	//강타입의 경우는 지정한 위젯만 들어올 수 있고 (로비 UI, 인게임 HUD)
	//약타입의 경우는 어느 위젯이든 가리킬 수 있음 (타이틀/팝업/오버레이)
	//보통 Class는 약타입, Instance는 강타입으로 사용
	
	//강 타입 참조
	UPROPERTY()
	class UHBLobbyWidget* LobbyWidget;

	//약타입의 경우 다음과 같이 선언함 (userwidget을 상속한 어느 위젯이든 가리킬 수 있음)
	//UPROPERTY()
	//UUserWidget* LobbyWidget = nullptr;

	//약 타입 참조
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	//강타입의 경우 다음과 같이 선언함 (HBTitleWidget만 들어올 수 있음)
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UHBTitleWidget> LobbyWidgetClass;

	//생성한 UI  목록
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> SpawnedWidgets;

	UPROPERTY()
	TObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	bool bExitRequested = false;
	bool bExitWasHost = false;
	bool bFriendInviteOpen = false;
};
