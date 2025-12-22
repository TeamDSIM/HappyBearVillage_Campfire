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
//DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>&, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//Host가 방 생성
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	//void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();

	//UI 바인딩
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	//FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

	//친구 초대로 접속용
	FOnSessionUserInviteAcceptedDelegate SessionUserInviteAcceptedDelegate;
	FDelegateHandle SessionUserInviteAcceptedDelegateHandle;




protected:

	UMultiplayerSessionsSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	//Callback
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	//void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnSessionUserInviteAccepted(bool bWasSuccessful, int32 ControllerId,
		FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	

private:
	IOnlineSessionPtr SessionInterface;

	//Delegate, Handle
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	//FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	//FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	//Create 호출 파라미터
	//@Todo : 최대 인원 설정
	int32 LastNumPublicConnections = 4;
	FString LastMatchType = TEXT("FreeForAll");

	bool bCreateSessionOnDestroy = false;

	//검색용 객체
	//TSharedPtr<FOnlineSessionSearch> SessionSearch;





};
