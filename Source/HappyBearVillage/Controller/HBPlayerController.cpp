// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPlayerController.h"
#include "Blueprint/Userwidget.h" 
#include "../UI/HBTitleWidget.h"
#include "Component/HBInGameHUDComponent.h"
#include "../UI/HBLobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/HBGameInstance.h"
#include "Engine/World.h"
#include "MultiplayerSessionsSubsystem.h"


AHBPlayerController::AHBPlayerController()
{
	//static ConstructorHelpers::FClassFinder<UHBLobbyWidget> HBLobbyWidgetRef(TEXT("/Game/Personal/LEE_J_S/UI/TestLobby.TestLobby_C"));
	//if (HBLobbyWidgetRef.Succeeded())
	//{
	//	HBLobbyWidgetClass = HBLobbyWidgetRef.Class();
	//}

	InGameHUDComponent = CreateDefaultSubobject<UHBInGameHUDComponent>(TEXT("InGameHUD"));
}

void AHBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("PlayerController BeginPlay"));

	//이 체크가 있어야 client 추가해도 오류 X
	//체크가 없으면 remote pc에서 createwidget를 호출하는데, localplayer 가 없으므로 설정 불가
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("no LocalController >>Playercontroller"));
		return;
	}

	ResetUI();
	SetupUI();

}

//Pawn Possess하라고 Server->Client 지정 후 호출되는 함수
//Travel 이후, Respawn 이후, Possess 갱신 시 호출

//void AHBPlayerController::BeginPlayingState()
//{
//	UE_LOG(LogTemp, Log, TEXT("BeginPlayingState Called"));
//
//	Super::BeginPlayingState();
//
//	if (!IsLocalController())
//	{
//		UE_LOG(LogTemp, Log, TEXT("not localcontroller"));
//		return;
//	}
//
//
//	UHBGameInstance* GI = GetGameInstance<UHBGameInstance>();
//
//	if (!GI)
//	{
//		UE_LOG(LogTemp, Log, TEXT("not gameinstance"));
//		return;
//	}
//	if (!GI->bNewMapLoad)
//	{
//		UE_LOG(LogTemp, Log, TEXT("not a new map"));
//		return;
//	}
//
//
//	ResetUI();
//	SetupUI(); 
//
//	GI->bNewMapLoad = false; 
//}







void AHBPlayerController::ResetUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call ResetUI"));
	RemoveUI();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	SetShowMouseCursor(false);
}

void AHBPlayerController::SetupUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call SetupUI"));
	if (!IsLocalController())
	{
		return;
	}

	//맵에 따라 어떤 UI를 띄울지 확인
	const FString Map = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	

	if (Map == TEXT("TestGameStartMap"))
	{
		CreateTitleUI();
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	}

	if (Map == TEXT("TestMap"))
	{
		if (InGameHUDComponent)
		{
			InGameHUDComponent->ActivateHUD(this);
		}
	}

	if (Map == TEXT("TestLobbyMap"))
	{
		CreateLobbyUI();
		//FInputModeGameOnly InputMode;
		//SetInputMode(InputMode);
		//SetShowMouseCursor(false);
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	}

	if (Map == TEXT("InGameMap"))
	{
		CreateLobbyUI();
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);

	}

}

void AHBPlayerController::CreateTitleUI()
{
	if (!IsLocalController() || TitleWidget)
	{
		return;
	}

	//약타입으로 생성
	UUserWidget* RawWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);

	//강타입으로 캐스팅
	TitleWidget = Cast<UHBTitleWidget>(RawWidget);
	
	TitleWidget->AddToViewport();

	SpawnedWidgets.Add(RawWidget);

}

void AHBPlayerController::CreateLobbyUI()
{
	UE_LOG(LogTemp, Log, TEXT("createlobbyuicalled"));

	if (!IsLocalController() || LobbyWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Not LobbyWidget"));
		return;
	}

	//약타입으로 생성
	UUserWidget* RawWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass);

	//강타입으로 캐스팅
	LobbyWidget = Cast<UHBLobbyWidget>(RawWidget);

	LobbyWidget->AddToViewport();

	SpawnedWidgets.Add(RawWidget);
}

void AHBPlayerController::RemoveUI()
{
	for (UUserWidget* W : SpawnedWidgets)
	{
		if (W)
		{
			W->RemoveFromParent();
		}
	}
	SpawnedWidgets.Empty();

	// 강타입 포인터도 정리
	// 강타입은 배열에 관리 X (같은 위젯을 중복 제거할수도 있음)
	TitleWidget = nullptr;
	LobbyWidget = nullptr;
}

void AHBPlayerController::StartGame()
{
	//호스트인 경우만 작동
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT HOST!!!"));
		return;
	}
	//@ Todo : 맵 이름 변경
	FString Map = TEXT("/Game/Maps/InGameMap");
		GetWorld()->ServerTravel(Map);
}

void AHBPlayerController::ExitGame()
{
	UE_LOG(LogTemp, Log, TEXT("ExitGame Called"));

	if (UGameInstance* GI = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (!MultiplayerSessionsSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("not MultiplayerSessionsSubsystem"));
		return;
	}


	//Host 인 경우
	if (HasAuthority())
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
		//@Todo : 맵 이름 변경
	FName Map = TEXT("/Game/Personal/LEE_J_S/Maps/TestGameStartMap");
	UGameplayStatics::OpenLevel(this, Map);
}