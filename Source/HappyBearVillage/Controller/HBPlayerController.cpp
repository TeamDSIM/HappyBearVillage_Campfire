// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPlayerController.h"
#include "Blueprint/Userwidget.h" 
#include "../UI/HBTitleWidget.h"
#include "Component/HBInGameHUDComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/HBGameInstance.h"



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

	else if (Map == TEXT("TestMap"))
	{
		if (InGameHUDComponent)
		{
			InGameHUDComponent->ActivateHUD(this);
		}
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
	//HBLobbyWidget = nullptr;
}


