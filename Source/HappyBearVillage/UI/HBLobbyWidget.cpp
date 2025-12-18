// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyWidget.h"
#include "Components/Button.h"
#include "Engine/GameInstance.h"
#include "MultiplayerSessionsSubsystem.h"

//화면에 나타날 준비가 모두 끝난 경우
void UHBLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameInstance* GI = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		//MultiCast Delegate 바인딩
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUObject(
			this, &ThisClass::OnCreateSessionComplete);

		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(
			this, &ThisClass::OnFindSessionsComplete);
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

}

void UHBLobbyWidget::HostButtonClicked()
{
	//UE_LOG(LogTemp, Log, TEXT("Host Made the Room"));

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(4, TEXT("FreeForAll"));
	}
}

void UHBLobbyWidget::JoinButtonClicked()
{

	UE_LOG(LogTemp, Log, TEXT("Client Joined the Room"));

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UHBLobbyWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("CreateSessionComplete: %s"), bWasSuccessful ? TEXT("Success") : TEXT("Fail"));

}

void UHBLobbyWidget::OnFindSessionsComplete(
    const TArray<FOnlineSessionSearchResult>& Results,
    bool bWasSuccessful)

{
    UE_LOG(LogTemp, Log, TEXT("OnFindSessionsComplete Called"));

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("FindSessions failed."));
        return;
    }

    if (Results.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("FindSessions succeeded but no results."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("FindSessions returned %d results"), Results.Num());

    // 내가 찾고 싶은 MatchType
    const FString DesiredMatchType = TEXT("FreeForAll");

    for (const FOnlineSessionSearchResult& Result : Results)
    {
        UE_LOG(LogTemp, Log, TEXT("Result Owner=%s, SessionId=%s, BuildUniqueId=%u, OpenSlots=%d, Ping=%d"),
            *Result.Session.OwningUserName,
            *Result.GetSessionIdStr(),
            Result.Session.SessionSettings.BuildUniqueId,
            Result.Session.NumOpenPublicConnections,
            Result.PingInMs);


        FString FoundMatchType;
        bool bHasMatchType = Result.Session.SessionSettings.Get(FName("MatchType"), FoundMatchType);

        if (!bHasMatchType)
        {
            continue; // MatchType 없는 방은 스킵
        }

        UE_LOG(LogTemp, Log, TEXT("Found session MatchType = %s"), *FoundMatchType);

        // MatchType이 내가 원하는 값이면 Join
        if (FoundMatchType == DesiredMatchType)
        {
            UE_LOG(LogTemp, Log, TEXT("Joining session with MatchType %s"), *FoundMatchType);

            MultiplayerSessionsSubsystem->JoinSession(Result);
            return;
        }
    }

    // 여기까지 왔다는 건, 조건에 맞는 방이 없다는 뜻
    UE_LOG(LogTemp, Warning, TEXT("No session with MatchType %s found"), *DesiredMatchType);
}
