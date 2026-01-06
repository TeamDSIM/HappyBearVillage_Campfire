// Fill out your copyright notice in the Description page of Project Settings.


#include "HBTitlePlayerController.h"

#include "UI/HBTitleWidget.h"

AHBTitlePlayerController::AHBTitlePlayerController() { }

void AHBTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("PlayerController BeginPlay"));

	//이 체크가 있어야 client 추가해도 오류 X
	//체크가 없으면 remote pc에서 createwidget를 호출하는데, localplayer 가 없으므로 설정 불가
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("No Local Controller >> PlayerController"));
		return;
	}

	ResetUI();
	SetupUI();
}

void AHBTitlePlayerController::ResetUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call ResetUI"));
	RemoveUI();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	SetShowMouseCursor(false);
}

void AHBTitlePlayerController::SetupUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call SetupUI"));
	if (!IsLocalController())
	{
		return;
	}

	CreateTitleUI();
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void AHBTitlePlayerController::RemoveUI()
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
}

void AHBTitlePlayerController::CreateTitleUI()
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
