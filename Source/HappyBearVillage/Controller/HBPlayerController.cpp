// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPlayerController.h"
#include "Blueprint/Userwidget.h" 
#include "../UI/HBTitleWidget.h"
#include "Component/HBInGameHUDComponent.h"
#include "../UI/HBLobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/HBGameInstance.h"
#include "Engine/World.h"
#include "MultiplayerSessionsSubsystem.h"
#include "EnhancedInputComponent.h"
#include "Component/HBMapWidgetComponent.h"
#include "Component/HBMinimapWidgetComponent.h"
#include "GameMode/HBVillageGameMode.h"
#include "UI/HBCharacterStatusWidgetComponent.h"
#include "UI/Map/HBMapWidget.h"


AHBPlayerController::AHBPlayerController()
{
	InGameHUDComponent = CreateDefaultSubobject<UHBInGameHUDComponent>(TEXT("InGameHUD"));
	MinimapWidgetComponent = CreateDefaultSubobject<UHBMinimapWidgetComponent>(TEXT("Minimap"));
	MapWidgetComponent = CreateDefaultSubobject<UHBMapWidgetComponent>(TEXT("Map"));

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleMapActionRef(TEXT("/Game/Character/Input/Action/IA_ToggleMap.IA_ToggleMap"));
	if (ToggleMapActionRef.Succeeded())
	{
		ToggleMapAction = ToggleMapActionRef.Object;
	}
}

void AHBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("PlayerController BeginPlay"));

	//이 체크가 있어야 client 추가해도 오류 X
	//체크가 없으면 remote pc에서 createwidget를 호출하는데, localplayer 가 없으므로 설정 불가
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Log, TEXT("no LocalController >>Playercontroller"));
		return;
	}

	ResetUI();
	SetupUI();

}

void AHBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (FriendInviteInputAction)
		{
			EIC->BindAction(FriendInviteInputAction, ETriggerEvent::Started,this, &AHBPlayerController::ToggleFriendInvite);
			EIC->BindAction(ToggleMapAction, ETriggerEvent::Started, this, &AHBPlayerController::ToggleMapWidget);
		}
	}
}

void AHBPlayerController::ResetUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call ResetUI"));
	RemoveUI();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	SetShowMouseCursor(false);
}

void AHBPlayerController::SetupUI()
{
	UE_LOG(LogTemp, Log, TEXT("Call SetupUI"));
	if (!IsLocalController())
	{
		return;
	}

	//맵에 따라 어떤 UI를 띄울지 확인
	const FString Map = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	

	if (Map == TEXT("TitleMap"))
	{
		CreateTitleUI();
		FInputModeUIOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
	}

	if (Map == TEXT("TestMap"))
	{
		if (InGameHUDComponent)
		{
			InGameHUDComponent->ActivateHUD(this);
		}
	}

	if (Map == TEXT("LobbyMap"))
	{

		CreateLobbyUI();
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
		//FInputModeUIOnly InputMode;
		//SetInputMode(InputMode);
		//SetShowMouseCursor(true);
	}

	if (Map == TEXT("InGameMap"))
	{
		InGameHUDComponent->ActivateHUD(this);
		MinimapWidgetComponent->CreateMinimapWidget(this);
		MapWidgetComponent->CreateMapWidget(this);
		
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
	}

}

void AHBPlayerController::CreateTitleUI()
{
	if (!IsLocalController() || TitleWidget)
	{
		return;
	}

	//약타입으로 생성
	UUserWidget* RawWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);

	//강타입으로 캐스팅
	TitleWidget = Cast<UHBTitleWidget>(RawWidget);
	
	TitleWidget->AddToViewport();

	SpawnedWidgets.Add(RawWidget);

}

void AHBPlayerController::CreateLobbyUI()
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

void AHBPlayerController::RemoveUI()
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
	TitleWidget = nullptr;
	LobbyWidget = nullptr;
}

void AHBPlayerController::ToggleFriendInvite()
{
	UE_LOG(LogTemp, Log, TEXT("ToggleFriendInvite Called"));

	if (!IsLocalController()) return;
	//로비 위젯이 아니면 이 UI 호출 X
	if (!LobbyWidget) return;

	bFriendInviteOpen = !bFriendInviteOpen;
	LobbyWidget->SetFriendInviteVisible(bFriendInviteOpen);

	//토글 켜졌을때
	if (bFriendInviteOpen)
	{
		bShowMouseCursor = true;

		// 로비에서 움직임/시점 막기
		SetIgnoreMoveInput(true);
		SetIgnoreLookInput(true);

		FInputModeGameAndUI Mode;
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
	}

	//토글 꺼졌을때
	else
	{
		bShowMouseCursor = false;

		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);

		FInputModeGameOnly Mode;
		SetInputMode(Mode);
	}
}

void AHBPlayerController::ToggleMapWidget()
{
	MapWidgetComponent->ToggleMapWidget();
}

void AHBPlayerController::StartGame()
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
		if (AHBVillageGameMode* GM = World->GetAuthGameMode<AHBVillageGameMode>())
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

void AHBPlayerController::ExitGame()
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


	//Host 인 경우
	if (HasAuthority())
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
		//@Todo : 맵 이름 변경
	FName Map = TEXT("/Game/Maps/TitleMap");
	UGameplayStatics::OpenLevel(this, Map);
}