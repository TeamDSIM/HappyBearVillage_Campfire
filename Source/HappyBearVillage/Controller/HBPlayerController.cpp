// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPlayerController.h"
#include "Blueprint/Userwidget.h" 
#include "../UI/HBTitleWidget.h"
#include "Component/HBInGameHUDComponent.h"
#include "../UI/HBLobbyWidget.h"
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

	//�� üũ�� �־�� client �߰��ص� ���� X
	//üũ�� ������ remote pc���� createwidget�� ȣ���ϴµ�, localplayer �� �����Ƿ� ���� �Ұ�
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("no LocalController >>Playercontroller"));
		return;
	}

	ResetUI();
	SetupUI();

}

//Pawn Possess�϶�� Server->Client ���� �� ȣ��Ǵ� �Լ�
//Travel ����, Respawn ����, Possess ���� �� ȣ��

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

	//�ʿ� ���� � UI�� ����� Ȯ��
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

	//��Ÿ������ ����
	UUserWidget* RawWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);

	//��Ÿ������ ĳ����
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

	//��Ÿ������ ����
	UUserWidget* RawWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass);

	//��Ÿ������ ĳ����
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

	// ��Ÿ�� �����͵� ����
	// ��Ÿ���� �迭�� ���� X (���� ������ �ߺ� �����Ҽ��� ����)
	TitleWidget = nullptr;
	LobbyWidget = nullptr;
}


