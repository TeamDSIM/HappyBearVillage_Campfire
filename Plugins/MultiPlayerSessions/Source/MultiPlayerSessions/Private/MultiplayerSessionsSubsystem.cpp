// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"

//static const FName NAME_GameSession = NAME_GameSession;

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	//Delegate 바인딩
	CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);


	//SessionInterface 획득
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}

}

//세션 생성 작업 시작 
void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	// 1. Session Inetrface가 유효한지 확인
	//OnlineSubsystem이 있는지 확인
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnCreateSessionComplete.Broadcast(false);
		return;
	}

	LastNumPublicConnections = NumPublicConnections;
	LastMatchType = MatchType;

	// 2. 기존 세션이 남아있는지 확인
	//이미 세션이 있다면 (호스트 중/이전 세션 남아있음) 처리
	if (FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession))
	{
		//Destroy 후 다시 Create

		//다시 만들 것이라는 플래그
		bCreateSessionOnDestroy = true;
		SessionInterface->DestroySession(NAME_GameSession);
		return;
	}

	// 3. Delegate에 등록
	// CreateSession이 비동기여서 작업이 끝났을 때 부르기 위해 필요
	CreateSessionCompleteDelegateHandle =
		SessionInterface->AddOnCreateSessionCompleteDelegate_Handle((CreateSessionCompleteDelegate));


	// 4. 세션 세팅
	FOnlineSessionSettings SessionSettings;

	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const bool bIsLAN = (Subsystem && Subsystem->GetSubsystemName() == "NULL");


	//Steam(온라인) ,Null OSS(로컬) 에 따라 다른 설정
	SessionSettings.bIsLANMatch = bIsLAN;

	//최대 인원
	SessionSettings.NumPublicConnections = NumPublicConnections;


	SessionSettings.bAllowJoinInProgress = true;

	// 다른 사람이 FindSessions으로 찾을수 있게 할지 (방 공개 여부)
	SessionSettings.bShouldAdvertise = true;

	//Steam 연동용
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	//Steam, Lobby 에서 자주 필요함
	SessionSettings.bUseLobbiesIfAvailable = true;

	//커스텀 키 (find,join 시 MatchType 필터링 시에 사용)
	SessionSettings.Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 5. NetId 가져오기
	//호스트 uniquenetid
	UWorld* World = GetWorld();

	// 6. 요청 보내기
	if (!World)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		MultiplayerOnCreateSessionComplete.Broadcast(false);
		return;
	}

	ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		MultiplayerOnCreateSessionComplete.Broadcast(false);
		return;
	}

	const FUniqueNetIdRepl& NetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
	if (!NetIdRepl.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		MultiplayerOnCreateSessionComplete.Broadcast(false);
		return;
	}

	const bool bCreateStarted = SessionInterface->CreateSession(*NetIdRepl.GetUniqueNetId(), NAME_GameSession, SessionSettings);
	if (!bCreateStarted)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}

}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnCreateSessionComplete.Broadcast(false);
		return;
	}

	FindSessionsCompleteDelegateHandle =
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->MaxSearchResults = MaxSearchResults;

	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const bool bIsLAN = (Subsystem && Subsystem->GetSubsystemName() == "NULL");
	SessionSearch->bIsLanQuery = bIsLAN;

	SessionSearch->QuerySettings.Set(FName("PRESENCE"), true, EOnlineComparisonOp::Equals);

	ULocalPlayer* LocalPlayer = GetWorld() ? GetWorld()->GetFirstLocalPlayerFromController() : nullptr;

	if (!LocalPlayer)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	const FUniqueNetIdRepl& NetIdRepl = LocalPlayer->GetPreferredUniqueNetId();

	if (!NetIdRepl.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	const bool bStarted = SessionInterface->FindSessions(*NetIdRepl.GetUniqueNetId(), SessionSearch.ToSharedRef());
	if (!bStarted)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
	ULocalPlayer* LocalPlayer = GetWorld() ? GetWorld()->GetFirstLocalPlayerFromController() : nullptr;
	if (!LocalPlayer)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	const FUniqueNetIdRepl& NetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
	if (!NetIdRepl.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	const bool bStarted = SessionInterface->JoinSession(*NetIdRepl.GetUniqueNetId(), NAME_GameSession, SearchResult);
	if (!bStarted)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

//Callback 부분, OSS가 작업이 끝났다고 부르는 함수
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 1. Delegate Handle 클리어
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	//UI에 결과 알리기
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);

	// 성공 시 ServerTravel("?listen") 요구사항
	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			//todo : 맵 위치 변경 시 주소 수정해줘야함
			// Listen 서버로 열기
			World->ServerTravel(TEXT("/Game/Personal/LEE_J_S/Maps/TestLobbyMap?listen"));
		}
	}
}


void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (!bWasSuccessful || !SessionSearch.IsValid())
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, true);

}
void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(Result);

	if (Result != EOnJoinSessionCompleteResult::Success) return;

	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		return;
	}

	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
	}
}
