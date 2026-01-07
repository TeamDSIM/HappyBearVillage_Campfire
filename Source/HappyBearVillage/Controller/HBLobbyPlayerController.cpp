// Fill out your copyright notice in the Description page of Project Settings.


#include "HBLobbyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameMode/HBLobbyGameMode.h"
#include "GameMode/HBVillageGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HBLobbyWidget.h"

AHBLobbyPlayerController::AHBLobbyPlayerController() { }

void AHBLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//이 체크가 있어야 client 추가해도 오류 X
	//체크가 없으면 remote pc에서 createwidget를 호출하는데, localplayer 가 없으므로 설정 불가
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("No LocalController >> PlayerController"));
		return;
	}

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerIMC)
				Subsystem->AddMappingContext(PlayerIMC, 0);
		}
	}

	ResetUI();
	SetupUI();
}

void AHBLobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (FriendInviteInputAction)
		{
			EIC->BindAction(FriendInviteInputAction, ETriggerEvent::Started,this, &AHBLobbyPlayerController::ToggleFriendInvite);
		}
	}
}

void AHBLobbyPlayerController::ResetUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call ResetUI"));
	RemoveUI();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	SetShowMouseCursor(false);
}

void AHBLobbyPlayerController::SetupUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call SetupUI"));
	if (!IsLocalController())
	{
		return;
	}

	CreateLobbyUI();
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);
	//FInputModeUIOnly InputMode;
	//SetInputMode(InputMode);
	//SetShowMouseCursor(true);
}

void AHBLobbyPlayerController::RemoveUI()
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
	LobbyWidget = nullptr;
}

void AHBLobbyPlayerController::CreateLobbyUI()
{
	UE_LOG(LogTemp, Log, TEXT("CreateLobbyUICalled"));

	if (!IsLocalController() || LobbyWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Not LobbyWidget"));
		return;
	}

	//약타입으로 생성
	UUserWidget* RawWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass);

	//강타입으로 캐스팅
	LobbyWidget = Cast<UHBLobbyWidget>(RawWidget);

	LobbyWidget->AddToViewport();

	SpawnedWidgets.Add(RawWidget);
}

void AHBLobbyPlayerController::ToggleFriendInvite()
{
	UE_LOG(LogTemp, Log, TEXT("ToggleFriendInvite Called"));

	if (!IsLocalController()) return;
	//로비 위젯이 아니면 이 UI 호출 X
	if (!LobbyWidget) return;

	bFriendInviteOpen = !bFriendInviteOpen;
	LobbyWidget->SetFriendInviteVisible(bFriendInviteOpen);
	
	if (bFriendInviteOpen)
	{
		//토글 켜졌을때
		bShowMouseCursor = true;

		// 로비에서 움직임/시점 막기
		SetIgnoreMoveInput(true);
		SetIgnoreLookInput(true);

		FInputModeGameAndUI Mode;
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
	}
	else
	{
		//토글 꺼졌을때
		bShowMouseCursor = false;

		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);

		FInputModeGameOnly Mode;
		SetInputMode(Mode);
	}
}

void AHBLobbyPlayerController::StartGame()
{
	//호스트인 경우만 작동
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT HOST!!!"));
		return;
	}
	
	// Seamless Travel을 활성화하기 위해 GameMode를 가져와 설정 확인
	if (UWorld* World = GetWorld())
	{
		if (AHBLobbyGameMode* GM = World->GetAuthGameMode<AHBLobbyGameMode>())
		{
			GM->bUseSeamlessTravel = true; // 이동 직전에 활성화
		}

		// 절대 경로보다는 상대 경로 형식을 권장하지만, 프로젝트 구조에 맞춰 수정
		// .umap 확장자를 제외한 경로를 사용합니다.
		FString MapPath = TEXT("/Game/Maps/InGameMap");
		World->ServerTravel(MapPath);
	}

	//@ Todo : 맵 이름 변경
	// FString Map = TEXT("/Game/Maps/InGameMap");
	// 	GetWorld()->ServerTravel(Map);
}

void AHBLobbyPlayerController::ExitGame()
{
	UE_LOG(LogTemp, Log, TEXT("ExitGame Called"));

	if (UGameInstance* GI = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GI->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (!MultiplayerSessionsSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("not MultiplayerSessionsSubsystem"));
		return;
	}

	if (bExitRequested)
	{
		return;
	}

	bExitRequested = true;
	bExitWasHost = HasAuthority();

	//  Destroy 완료 콜백 구독 (한 번만)
	MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveAll(this);
	MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddUObject(
		this, &ThisClass::HandleExitDestroySessionComplete
	);

	//  Host/Client 모두 로컬 세션 정리는 DestroySession으로 통일
	MultiplayerSessionsSubsystem->DestroySession();
}

void AHBLobbyPlayerController::HandleExitDestroySessionComplete(bool bWasSuccessful)
{
	if (!bExitRequested)
	{
		return;
	}

	bExitRequested = false;

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveAll(this);
	}

	// 실패해도 Title로 빠져나가고 싶으면 그대로 진행
	if (bExitWasHost && HasAuthority())
	{
		//  Host: 서버가 Title로 ServerTravel → 모든 클라이언트가 같이 따라감
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(TEXT("/Game/Maps/TitleMap"));
		}
		return;
	}

	//  Client: 본인만 Title로 이동
	UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Maps/TitleMap")));
}
