// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.generated.h"
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool/*bWasSuccessful*/);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();

	//Host가 방 생성
	void CreateSession(int32 NumPublicConnections, FString MatchType);

	//UI 바인딩
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;

protected:
	//Callback
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;

	//Delegate, Handle
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	//Create 호출 파라미터
	int32 LastNumPublicConnections = 4;
	FString LastMatchType = TEXT("FreeForAll");

	bool bCreateSessionOnDestroy = false;





};
