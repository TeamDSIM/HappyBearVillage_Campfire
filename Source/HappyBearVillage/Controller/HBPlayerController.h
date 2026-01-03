// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "HBPlayerController.generated.h"

class UHBTitleWidget;
class UHBLobbyWidget;
class UMultiplayerSessionsSubsystem;

class UInputMappingContext;
//class UInputAction;

UCLASS()
class HAPPYBEARVILLAGE_API AHBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	AHBPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//키 입력
	// IMC는 이미 Character에서 추가해서 필요 X
	// BP에서 지정 
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FriendInviteInputAction;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ToggleMapAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHBInGameHUDComponent> InGameHUDComponent;
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBMinimapWidgetComponent> MinimapWidgetComponent;
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBMapWidgetComponent> MapWidgetComponent;

private:
	//강타입의 경우는 지정한 위젯만 들어올 수 있고 (로비 UI, 인게임 HUD)
	//약타입의 경우는 어느 위젯이든 가리킬 수 있음 (타이틀/팝업/오버레이)
	//보통 Class는 약타입, Instance는 강타입으로 사용


	//강 타입 참조
	UPROPERTY()
	UHBTitleWidget* TitleWidget;
	UPROPERTY()
	UHBLobbyWidget* LobbyWidget;

	//약타입의 경우 다음과 같이 선언함 (userwidget을 상속한 어느 위젯이든 가리킬 수 있음)
	//UPROPERTY()
	//UUserWidget* LobbyWidget = nullptr;

	//약 타입 참조
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TitleWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	//강타입의 경우 다음과 같이 선언함 (HBTitleWidget만 들어올 수 있음)
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UHBTitleWidget> LobbyWidgetClass;

	//생성한 UI  목록
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> SpawnedWidgets;

	UPROPERTY()
	TObjectPtr< UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	void ToggleFriendInvite();
	void ToggleMapWidget();

	bool bFriendInviteOpen = false;

public:

	void ResetUI();
	void SetupUI();
	void RemoveUI();

	void CreateTitleUI();
	void CreateLobbyUI();
	
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ExitGame();

};
