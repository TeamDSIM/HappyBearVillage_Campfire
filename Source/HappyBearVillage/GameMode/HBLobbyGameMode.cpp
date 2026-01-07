// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyGameMode.h"

#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"

AHBLobbyGameMode::AHBLobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Character/Blueprint/BP_HBCharacterPlayer.BP_HBCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Controller/BP_HBLobbyPlayerController.BP_HBLobbyPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = AHBMafiaGameState::StaticClass();
	PlayerStateClass = AHBPlayerState::StaticClass();

	bUseSeamlessTravel = false;
}
