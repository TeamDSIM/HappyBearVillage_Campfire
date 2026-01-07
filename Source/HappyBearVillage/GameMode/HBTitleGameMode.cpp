// Fill out your copyright notice in the Description page of Project Settings.


#include "HBTitleGameMode.h"

AHBTitleGameMode::AHBTitleGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Controller/BP_HBTitlePlayerController.BP_HBTitlePlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
