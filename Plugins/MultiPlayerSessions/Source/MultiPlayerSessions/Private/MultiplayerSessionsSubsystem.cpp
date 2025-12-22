// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"

#include "Engine/Engine.h"
#include "Engine/GameEngine.h"
#include "Engine/NetDriver.h"

//static const FName NAME_GameSession = NAME_GameSession;

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	//Delegate 바인딩
	CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	//FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);


	//SessionInterface 획득
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}

}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Log, TEXT("Initialize Called"));

	Super::Initialize(Collection);

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}

	if (SessionInterface.IsValid())
	{
		SessionUserInviteAcceptedDelegate =
			FOnSessionUserInviteAcceptedDelegate::CreateUObject(
				this, &ThisClass::OnSessionUserInviteAccepted);

		SessionUserInviteAcceptedDelegateHandle =
			SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(
				SessionUserInviteAcceptedDelegate);
	}

	//확인용 로그
	UE_LOG(LogTemp, Warning, TEXT("=== MultiplayerSessionsSubsystem Initialize ==="));

	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine is NULL"));
		return;
	}

	const UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
	if (!GameEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine is not UGameEngine"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("NetDriverDefinitions count = %d"),
		GameEngine->NetDriverDefinitions.Num());

	for (const FNetDriverDefinition& Def : GameEngine->NetDriverDefinitions)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("NetDriverDef: DefName=%s | DriverClass=%s | Fallback=%s"),
			*Def.DefName.ToString(),
			*Def.DriverClassName.ToString(),
			*Def.DriverClassNameFallback.ToString()
		);
	}


	UClass* SteamSocketsDriverClass = StaticLoadClass(
		UNetDriver::StaticClass(),
		nullptr,
		TEXT("/Script/SteamSockets.SteamSocketsNetDriver")
	);

	UClass* SteamSocketsConnClass = StaticLoadClass(
		UObject::StaticClass(),
		nullptr,
		TEXT("/Script/SteamSockets.SteamSocketsNetConnection")
	);

	UE_LOG(LogTemp, Warning, TEXT("SteamSocketsNetDriver class load: %s"),
		SteamSocketsDriverClass ? TEXT("OK") : TEXT("FAIL"));
	UE_LOG(LogTemp, Warning, TEXT("SteamSocketsNetConnection class load: %s"),
		SteamSocketsConnClass ? TEXT("OK") : TEXT("FAIL"));

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

	// BuildUniqueId를 명시적으로 맞춤 (server 000000으로 나오는 버그 수정용)
	SessionSettings.BuildUniqueId = GetBuildUniqueId();

	// 위 과정 확인용 로그
	UE_LOG(LogTemp, Log,
		TEXT("Host CreateSession: GetBuildUniqueId()=%u, Settings.BuildUniqueId=%u"),
		GetBuildUniqueId(),
		SessionSettings.BuildUniqueId);


	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	const bool bIsLAN = (Subsystem && Subsystem->GetSubsystemName() == "NULL");


	//Steam(온라인) ,Null OSS(로컬) 에 따라 다른 설정
	SessionSettings.bIsLANMatch = bIsLAN;

	//최대 인원
	SessionSettings.NumPublicConnections = NumPublicConnections;


	SessionSettings.bAllowJoinInProgress = true;

	// 다른 사람이 FindSessions으로 찾을수 있게 할지 (방 공개 여부)
	SessionSettings.bShouldAdvertise = true;

	SessionSettings.bIsDedicated = false;   // 

	//Steam 연동용
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	//Steam, Lobby 에서 자주 필요함
	SessionSettings.bUseLobbiesIfAvailable = true;

	//커스텀 키 (find,join 시 MatchType 필터링 시에 사용)
	SessionSettings.Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 키워드 광고 추가
	SessionSettings.Set(
		FName(TEXT("HB_Keywords")),
		FString(TEXT("HBVillage")),
		EOnlineDataAdvertisementType::ViaOnlineService
	);

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

	UE_LOG(LogTemp, Log,
		TEXT("Host SessionSettings: Presence=%d, Lobbies=%d, Dedicated=%d, Advertise=%d, BuildId=%u"),
		SessionSettings.bUsesPresence,
		SessionSettings.bUseLobbiesIfAvailable,
		SessionSettings.bIsDedicated,
		SessionSettings.bShouldAdvertise,
		SessionSettings.BuildUniqueId
	);

}

//세션 찾기

//void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
//{
//	UE_LOG(LogTemp, Log, TEXT("FindSessions Called"));
//	// 1. 세션이 유효한지 확인
//	if (!SessionInterface.IsValid())
//	{
//		//MultiplayerOnCreateSessionComplete.Broadcast(false);
//		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
//		return;
//	}
//
//	// 2. 델리게이트 등록
//	FindSessionsCompleteDelegateHandle =
//		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
//
//	// 3. 검색 객체 생성 및 설정 추가
//	SessionSearch = MakeShared<FOnlineSessionSearch>();
//	SessionSearch->MaxSearchResults = MaxSearchResults;
//
//	const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
//	// const bool bIsLAN = (Subsystem && Subsystem->GetSubsystemName() == "NULL");
//	// SessionSearch->bIsLanQuery = bIsLAN;
//	SessionSearch->bIsLanQuery = false;
//
//	SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCE")), true, EOnlineComparisonOp::Equals);
//
//	SessionSearch->QuerySettings.Set(
//		FName(TEXT("LOBBIES")),
//		true,
//		EOnlineComparisonOp::Equals
//	);
//
//	//SessionSearch->QuerySettings.Set(
//	//	FName(TEXT("HB_Keywords")),
//	//	FString(TEXT("HBVillage")),
//	//	EOnlineComparisonOp::In /*Equals*/
//	//);
//
//
//	// 4. LocalPlayer 가져옴 + 못가져온 경우
//	ULocalPlayer* LocalPlayer = GetWorld() ? GetWorld()->GetFirstLocalPlayerFromController() : nullptr;
//
//	if (!LocalPlayer)
//	{
//		UE_LOG(LogTemp, Log, TEXT("There is no LocalPlayer. >>FindSessions"));
//		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
//		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
//		return;
//	}
//
//	// 5. Net ID 가져옴 + 못가져온 경우
//	const FUniqueNetIdRepl& NetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
//
//	if (!NetIdRepl.IsValid())
//	{
//		UE_LOG(LogTemp, Log, TEXT("Not valid NedIdRepl(Steam ID). >>FindSessions"));
//		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
//		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
//		return;
//	}
//
//	UE_LOG(LogTemp, Log,
//		TEXT("Find Debug: bIsLanQuery=%d | Presence=%s | Lobbies=%s"),
//		SessionSearch->bIsLanQuery,
//		SessionSearch->QuerySettings.SearchParams.Contains(FName(TEXT("PRESENCE"))) ? TEXT("SET") : TEXT("NO"),
//		SessionSearch->QuerySettings.SearchParams.Contains(FName(TEXT("LOBBIES"))) ? TEXT("SET") : TEXT("NO")
//	);
//
//
//	// 6. Session 찾기 + 못찾은 경우
//	const bool bStarted = SessionInterface->FindSessions(*NetIdRepl.GetUniqueNetId(), SessionSearch.ToSharedRef());
//
//
//	if (!bStarted)
//	{
//		UE_LOG(LogTemp, Log, TEXT("cannot Start FindSession. >>FindSessions"));
//		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
//		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
//		return;
//	}
//}

//세션 조인

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{

	UE_LOG(LogTemp, Log, TEXT("JoinSession Called"));

	JoinSessionCompleteDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	// 1. ssionInterface가 유효한지 체크
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// 2. LocalPlayer 가져옴 + 못가져온 경우
	ULocalPlayer* LocalPlayer = GetWorld() ? GetWorld()->GetFirstLocalPlayerFromController() : nullptr;
	if (!LocalPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("There is no LocalPlayer. >>JoinSession"));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// 5. Net ID 가져옴 + 못가져온 경우
	const FUniqueNetIdRepl& NetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
	if (!NetIdRepl.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Not valid NedIdRepl(Steam ID). >>JoinSession"));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// 6. Session 조인 + 못찾은 경우
	const bool bStarted = SessionInterface->JoinSession(*NetIdRepl.GetUniqueNetId(), NAME_GameSession, SearchResult);
	if (!bStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("cannot Start FindSession. >>JoinSession"));
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

//세션 파괴 (나가기)
void UMultiplayerSessionsSubsystem::DestroySession()
{
	//host 가 아닌 경우
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	//host 인 경우 방 삭제 후 나가기
	SessionInterface->DestroySession(NAME_GameSession);
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
		FNamedOnlineSession* NamedSession =
			SessionInterface->GetNamedSession(NAME_GameSession);

		UE_LOG(LogTemp, Log,
			TEXT("After CreateSession: NamedSession=%s"),
			NamedSession ? TEXT("VALID") : TEXT("NULL"));

		const FOnlineSessionSettings* Settings =
			SessionInterface->GetSessionSettings(NAME_GameSession);

		if (Settings)
		{
			FString KeywordValue;
			const bool bHasKeyword =
				Settings->Get(FName(TEXT("HB_Keywords")), KeywordValue);

			UE_LOG(LogTemp, Log,
				TEXT("Host Keyword Debug: HasHB_Keywords=%d Value=%s"),
				bHasKeyword,
				bHasKeyword ? *KeywordValue : TEXT("NONE")
			);
		}



		if (UWorld* World = GetWorld())
		{
			//todo : 맵 위치 변경 시 주소 수정해줘야함
			// Listen 서버로 열기
			World->ServerTravel(TEXT("/Game/Personal/LEE_J_S/Maps/TestLobbyMap?listen"));
		}
	}


}


// 세션 찾기 완료한 경우

//void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
//{
//	UE_LOG(LogTemp, Log, TEXT("OnFindSessionsComplete called"));
//	// 1. 세션이 유효한지 확인
//	if (SessionInterface.IsValid())
//	{
//		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
//	}
//
//	// 2. 검색 성공 여부와 유효성 검사
//	if (!bWasSuccessful || !SessionSearch.IsValid())
//	{
//		UE_LOG(LogTemp, Log, TEXT("fail to search session. >>OnFindSessionsComplete"));
//		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
//		return;
//	}
//
//	// 3. 성공 시 broadcast
//	MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, true);
//
//}

//세션 조인 완료한 경우

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Error,
		TEXT("### OnJoinSessionComplete CALLED | Result=%d | Session=%s"),
		(int32)Result, *SessionName.ToString());

	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}


	MultiplayerOnJoinSessionComplete.Broadcast(Result);

    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("JoinSession failed: %d"), (int32)Result);
        return;
    }

	// 접속 주소 해석 ((Host의 주소 해석)
	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		UE_LOG(LogTemp, Log, TEXT("Cannot Resolve connect String >>OnJoinSessionComplete"));
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("ConnectString = [%s]"), *ConnectString);

	// Client 가 현재 맵을 떠나 Host가 연 Listen Server 맵으로 이동
	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Move to Host Map. >>OnJoinSessionComplete"));
		PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
	}
}


void UMultiplayerSessionsSubsystem::OnSessionUserInviteAccepted(
	bool bWasSuccessful,
	int32 ControllerId,
	FUniqueNetIdPtr UserId,
	const FOnlineSessionSearchResult& InviteResult)
{
	UE_LOG(LogTemp, Warning, TEXT("InviteAccepted: success=%d, Lobby/SessionId=%s"),
		bWasSuccessful, *InviteResult.GetSessionIdStr());

	if (!bWasSuccessful)
	{
		return;
	}

	JoinSession(InviteResult);
}


void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}