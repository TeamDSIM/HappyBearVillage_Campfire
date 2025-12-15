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

	HBLobbyWidget = CreateWidget<UHBLobbyWidget>(this, HBLobbyWidgetClass);

	if (HBLobbyWidget)
	{
		HBLobbyWidget->LobbySetup();
	}
}
