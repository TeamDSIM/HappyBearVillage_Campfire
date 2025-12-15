// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPlayerController.h"
#include "Blueprint/Userwidget.h" 
#include "../UI/HBLobbyWidget.h"



AHBPlayerController::AHBPlayerController()
{
	//static ConstructorHelpers::FClassFinder<UHBLobbyWidget> HBLobbyWidgetRef(TEXT("/Game/Personal/LEE_J_S/UI/TestLobby.TestLobby_C"));
	//if (HBLobbyWidgetRef.Succeeded())
	//{
	//	HBLobbyWidgetClass = HBLobbyWidgetRef.Class();
	//}
}

void AHBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	if (!HBLobbyWidget)
	{
		HBLobbyWidget = CreateWidget<UHBLobbyWidget>(this, HBLobbyWidgetClass);
		HBLobbyWidget->AddToViewport();

	}

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(HBLobbyWidget->TakeWidget());
	SetInputMode(InputMode);
	SetShowMouseCursor(true);

	//HBLobbyWidget->LobbySetup();

}
