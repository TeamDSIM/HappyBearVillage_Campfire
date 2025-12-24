// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterPlayer.h"

#include "EngineUtils.h"
#include "InputMappingContext.h"
#include "Animation/HBPlayerCharacterAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/GameStateBase.h"
#include "GameMode/HBVillageGameMode.h"
#include "Interface/HBInteractableInterface.h"
#include "Net/UnrealNetwork.h"
#include "Stat/HBPlayerStatComponent.h"
#include "Subsystem/HBGameFlowSubsystem.h"
#include "Subsystem/HBGameVoteSubsystem.h"
#include "UI/HBUserHUDWidget.h"
#include "GameState/HBMafiaGameState.h"

AHBCharacterPlayer::AHBCharacterPlayer()
{
	// FPS 메쉬 설정
	FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMeshComponent->SetupAttachment(GetMesh());
	FPSMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FPSMeshComponent->SetOnlyOwnerSee(true);

	FPSMeshComponent->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	FPSMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 290.f));


	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	FPSMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	// FPS 카메라 설정
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(FPSMeshComponent);

	// 카메라 오프셋 구해서 머리에 붙이기
	FPSCameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, -75.0f, 560.0f), FRotator(0.0f, 90.0f, -90.0f));
	FPSCameraComponent->bUsePawnControlRotation = true;

	FPSCameraComponent->bEnableFirstPersonFieldOfView = true;
	FPSCameraComponent->bEnableFirstPersonScale = true;
	FPSCameraComponent->FirstPersonFieldOfView = FPSFieldOfView;
	FPSCameraComponent->FirstPersonScale = FPSViewScale;

	// 향상된 입력 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(
		TEXT("/Game/Character/Input/IMC_Player.IMC_Player"));
	if (InputMappingContextRef.Succeeded())
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(
		TEXT("/Game/Character/Input/Action/IA_Move.IA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(
		TEXT("/Game/Character/Input/Action/IA_Attack.IA_Attack"));
	if (AttackActionRef.Succeeded())
	{
		AttackAction = AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractionActionRef(
		TEXT("/Game/Character/Input/Action/IA_Interaction.IA_Interaction"));
	if (InteractionActionRef.Succeeded())
	{
		InteractionAction = InteractionActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseLookActionRef(
		TEXT("/Game/Character/Input/Action/IA_MouseLook.IA_MouseLook"));
	if (MouseLookActionRef.Succeeded())
	{
		MouseLookAction = MouseLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(
		TEXT("/Game/Character/Input/Action/IA_Jump.IA_Jump"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	// @PHYTODO : 임시 직업 분배
	static ConstructorHelpers::FObjectFinder<UInputAction> StartActionRef(
		TEXT("/Game/Character/Input/Action/IA_Start.IA_Start"));
	if (StartActionRef.Succeeded())
	{
		StartAction = StartActionRef.Object;
	}

	// 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(
		TEXT("/Game/Assets/Character/Mesh/Bear_Add_Mat.Bear_Add_Mat"));
	if (MeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FPSMeshRef(
		TEXT("/Game/Assets/Character/Mesh/Bear_Arm_fix1.Bear_Arm_fix1"));
	if (FPSMeshRef.Succeeded())
	{
		FPSMeshComponent->SetSkeletalMesh(FPSMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UHBPlayerCharacterAnimInstance> PlayerAnimInstanceRef(
		TEXT("/Game/Character/Anim/ABP_PlayerCharacter.ABP_PlayerCharacter_C"));
	if (PlayerAnimInstanceRef.Succeeded())
	{
		AnimInstanceClass = PlayerAnimInstanceRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(
		TEXT("/Game/Character/Anim/AM_CharacterAttack.AM_CharacterAttack"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}

	// 캐릭터 메시 설정
	GetMesh()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	FPSMeshComponent->SetAnimInstanceClass(AnimInstanceClass);

	// 1인칭 무기 메시 설정
	FPSCurrentWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FPSWeapon"));
	FPSCurrentWeapon->SetupAttachment(FPSMeshComponent, TEXT("RightHandWeaponSocket"));
	FPSCurrentWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	FPSCurrentWeapon->SetIsReplicated(true);
	FPSCurrentWeapon->SetOnlyOwnerSee(true);
	FPSCurrentWeapon->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

	// 공격 가능 여부 초기화
	bCanAttack = true;
}

void AHBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	/* ================= Night Flow : Game Start Init ================= */
	if (HasAuthority())
	{
		// 게임 시작 시에만 사과(외출권) 초기화
		Stamina = MaxStamina;
		bExitedHouseThisNight = false;
		NightState = EPlayerNightState::InHouse;
	}
	/* ================================================================ */

	DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);

	// InputMappingContext 설정
	// @PHYTODO : 이거 PossessedBy 로 옮겨줘야 함
	// APlayerController* PlayerController = Cast<APlayerController>(GetController());
	// if (PlayerController)
	// {
	// 	UEnhancedInputLocalPlayerSubsystem* Subsystem =
	// 		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	// 	if (Subsystem)
	// 	{
	// 		Subsystem->AddMappingContext(InputMappingContext, 0);
	// 	}
	// }


	if (IsLocallyControlled())
	{
		LastControlRotation = GetControlRotation();

		GetWorldTimerManager().SetTimer(
			InteracticonTraceTimerHandle,
			this,
			&AHBCharacterPlayer::InteractionTraceTick,
			InteractionTraceInterval,
			true
		);
	}
}

void AHBCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if (IsLocallyControlled())
	// {
	// 	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	// 	if (PlayerController)
	// 	{
	// 		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	// 		if (LocalPlayer)
	// 		{
	// 			UEnhancedInputLocalPlayerSubsystem* Subsystem =
	// 				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	// 			if (Subsystem)
	// 			{
	// 				Subsystem->AddMappingContext(InputMappingContext, 0);
	// 			}
	// 		}
	// 	}
	// }
}

void AHBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Move);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Attack);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this,
		                                   &AHBCharacterPlayer::Interaction);

		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &AHBCharacterPlayer::MouseLook);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// @PHYTODO : 임시 직업 분배
		EnhancedInputComponent->BindAction(StartAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Start);
	}
}

void AHBCharacterPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBCharacterPlayer, PlayerColor)
}

void AHBCharacterPlayer::Move(const FInputActionValue& Value)
{
	UWorld* World = Cast<UWorld>(GetWorld());
	if (!World)
	{
		return;
	}

	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	// 현재 Phase 가 Day(광장 이동 및 초기화 단계) 또는 Discussion(토론 단계) 이면 이동 방지
	if (GameState->CurrentPhase == EGamePhase::Day || GameState->CurrentPhase == EGamePhase::Discussion)
	{
		return;
	}

	const FVector2D MovementValue = Value.Get<FVector2D>();
	if (Controller)
	{
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementValue.X);

		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementValue.Y);
	}
}

// 공격 구현
void AHBCharacterPlayer::Attack()
{
	// 무기를 끼고있지 않으면 반환
	if (!bWeaponEquipped)
	{
		return;
	}

	if (bCanAttack)
	{
		if (!HasAuthority())
		{
			// 공격 중 공격 못하게 막음
			bCanAttack = false;

			// 공격 애니메이션 종료 시 실행될 델리게이트 함수 바인딩
			FTimerHandle Handle;

			GetWorldTimerManager().SetTimer(
				Handle,
				FTimerDelegate::CreateLambda(
					[&]()
					{
						bCanAttack = true;
					}
				), AttackTime, false, -1.f
			);

			// 클라이언트의 애니메이션 재생
			PlayAttackAnimation();
		}

		// ServerRPC 공격 실행
		ServerRPCAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void AHBCharacterPlayer::Interaction()
{
	UWorld* World = Cast<UWorld>(GetWorld());
	if (!World)
	{
		return;
	}

	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	// 현재 Phase 가 난투 Phase 가 아니면 상호작용 하지 못하도록 방지
	if (GameState->CurrentPhase != EGamePhase::Fight && GameState->CurrentPhase != EGamePhase::Lobby)
	{
		return;
	}

	if (IsLocallyControlled())
	{
		if (InteractionTarget != nullptr)
		{
			IHBInteractableInterface* InteractionActor = Cast<IHBInteractableInterface>(InteractionTarget);
			if (InteractionActor)
			{
				InteractionActor->Interact(this);
			}
		}

		InteractionTarget = nullptr;
	}
}

void AHBCharacterPlayer::MouseLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AHBCharacterPlayer::Start()
{
	// 서버이면 직업 분배
	if (!HasAuthority())
	{
		return;
	}

	ServerRPCStart();
}

void AHBCharacterPlayer::OnRep_PlayerColor()
{
	SetRandomBaseColor();
}

void AHBCharacterPlayer::SetRandomBaseColor()
{
	// DynamicMaterial 보장
	if (!DynamicMaterial && GetMesh())
	{
		DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
	}

	if (DynamicMaterial)
	{
		// 서버일때만 랜덤 색상 변수 생성
		if (HasAuthority())
		{
			PlayerColor = FLinearColor::MakeRandomColor();

			// PlayerState 의 Color 값 동기화
			AHBPlayerState* PawnState = Cast<AHBPlayerState>(GetPlayerState());
			if (PawnState)
			{
				PawnState->SyncPlayerColorFromPlayerStat(PlayerColor);
			}
		}

		// CharacterBaseColor 변수에 RandomColor 변수 부여
		DynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), PlayerColor);
	}
}

void AHBCharacterPlayer::ResetBaseColor()
{
	// DynamicMaterial 보장
	if (!DynamicMaterial && GetMesh())
	{
		DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
	}

	if (DynamicMaterial)
	{
		// 서버일때만 랜덤 색상 변수 생성
		if (HasAuthority())
		{
			PlayerColor = FLinearColor::White;
		}

		// CharacterBaseColor 변수에 RandomColor 변수 부여
		DynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), PlayerColor);
	}
}

void AHBCharacterPlayer::InteractionTraceTick()
{
	UWorld* World = Cast<UWorld>(GetWorld());
	if (!World)
	{
		return;
	}

	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	// 현재 Phase 가 난투 Phase 가 아니면 상호작용하지 않으므로 판별 실행 X
	if (GameState->CurrentPhase != EGamePhase::Fight && GameState->CurrentPhase != EGamePhase::Lobby)
	{
		return;
	}

	const FRotator CurrentRotation = GetControlRotation();

	// 시선 변화 체크 시 기준치를 넘지 않았으면 스킵
	if (CurrentRotation.Equals(LastControlRotation, ViewAngleThreshold))
	{
		return;
	}

	LastControlRotation = CurrentRotation;

	FVector Start = FPSCameraComponent->GetComponentLocation();
	FVector End = Start + (FPSCameraComponent->GetForwardVector() * InteractionDistance);

	// 트레이스 진행
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_GameTraceChannel1,
		Params
	);

	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		InteractionTarget = nullptr;
		return;
	}

	IHBInteractableInterface* Interactable = Cast<IHBInteractableInterface>(HitActor);
	if (Interactable)
	{
		if (Interactable->CanInteract(this))
		{
			InteractionTarget = HitActor;
		}
	}
}

void AHBCharacterPlayer::SetWeaponMesh()
{
	Super::SetWeaponMesh();

	if (bWeaponEquipped == true)
	{
		if (WeaponMesh != nullptr)
		{
			if (FPSCurrentWeapon->GetStaticMesh() == nullptr)
			{
				FPSCurrentWeapon->SetStaticMesh(WeaponMesh);
			}
		}
	}

	else
	{
		if (FPSCurrentWeapon->GetStaticMesh() != nullptr)
		{
			FPSCurrentWeapon->SetStaticMesh(nullptr);
		}
	}
}

void AHBCharacterPlayer::AttackHitCheck()
{
	// 현재 폰이 접속된 플레이어 클라이언트에 의해 제어중인지 확인
	if (IsLocallyControlled())
	{
		// 충돌 판정 구하기
		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		// 공격 관련 스탯 구하기 (사거리, 범위, 데미지 등)
		float AttackRange = Stat->GetBaseStat().AttackRange;
		float AttackRadius = Stat->GetBaseStat().AttackRadius;
		float AttackDamage = Stat->GetBaseStat().AttackDamage;

		// 플레이어 Forward 벡터, 공격 시작/끝 지점 벡터 구하기
		FVector Forward = GetActorForwardVector();
		FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
		FVector End = Start + GetActorForwardVector() * AttackRange;

		// Sweep 으로 충돌
		// @Todo: single 할지, multi 할지 (일단 single로 구현)
		bool HitDetected = GetWorld()->SweepSingleByChannel(
			OutHitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(AttackRadius),
			Params
		);

		// 충돌 체크 진행 시간 구함
		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		// 서버가 아니라면
		if (!HasAuthority())
		{
			// 충돌 했다면
			if (HitDetected)
			{
				// 서버RPC 로 충돌 노티파이 전송
				ServerRPCNotifyHit(OutHitResult, HitCheckTime);
			}

			// 충돌 안했다면
			else
			{
				// 서버RPC 로 미스 노티파이 전송
				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
			}
		}

		// 서버라면
		else
		{
			// 판정 범위 디버그 표시
			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			DrawDebugAttackRange(DebugColor, Start, End, Forward);

			// 충돌 했다면
			if (HitDetected)
			{
				// 데미지 판정
				AttackHitConfirm(OutHitResult.GetActor());
			}
		}
	}
}

void AHBCharacterPlayer::AttackHitConfirm(AActor* HitActor)
{
	// 서버라면
	if (HasAuthority())
	{
		// 공격 데미지 가져오기
		float AttackDamage = Stat->GetBaseStat().AttackDamage;

		// 부딫힌 대상에게 TakeDamage 로 데미지 전달
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);

		AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
		if (GameState && GameState->CurrentPhase == EGamePhase::Fight)
		{
			UHBGameVoteSubsystem* VoteSubsystem = GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
			if (VoteSubsystem)
			{
				// @PHYTODO : Top3 갱신 로직
			}
		}
	}
}

void AHBCharacterPlayer::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd,
                                              FVector Forward)
{
#if ENABLE_DRAW_DEBUG

	const float AttackRange = Stat->GetBaseStat().AttackRange;
	const float AttackRadius = Stat->GetBaseStat().AttackRadius;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
	                 FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void AHBCharacterPlayer::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult, float HitCheckTime)
{
	// 충돌한 액터 값을 가져옴
	AActor* HitActor = HitResult.GetActor();

	// 액터 값이 존재하면
	if (HitActor)
	{
		// 충돌 발생 위치, 충돌 발생한 액터의 바운딩 박스, 바운딩 박스의 가운데 지점
		FVector HitLocation = HitResult.Location;
		FBox HitBox = HitActor->GetComponentsBoundingBox();
		FVector ActorBoxCenter = HitBox.GetCenter();

		// 충돌 발생 위치와 바운딩 박스의 중간 지점이 충돌 체크 최소 거리보다 가까우면
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			// 충돌 시 이벤트 진행
			AttackHitConfirm(HitActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HitTest Reject"));
		}

		// 충돌 시 Notify 이니 디버그 컬러가 Green 만 넣어도 상관없긴 함
		DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd,
		                     HitActor->GetActorForwardVector());
	}
}

bool AHBCharacterPlayer::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitCheckTime)
{
	// 마지막 공격 시작 시간이 0 (처음 공격)
	if (LastAttackStartTime == 0.f)
	{
		return true;
	}
	// 충돌 체크 시간이랑 마지막 공격한 시간의 차이가 최소 체크 간격보다 커야 체크 진행
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
}

void AHBCharacterPlayer::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart,
                                                            FVector_NetQuantize TraceEnd,
                                                            FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	// 충돌 실패 시 판정 범위 디버그 그리기
	DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}

bool AHBCharacterPlayer::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd,
                                                      FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	// 마지막 공격 시작 시간이 0 (처음 공격)
	if (LastAttackStartTime == 0.f)
	{
		return true;
	}
	// 충돌 체크 시간이랑 마지막 공격한 시간의 차이가 최소 체크 간격보다 커야 체크 진행
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
}

void AHBCharacterPlayer::PlayAttackAnimation()
{
	// 캐릭터 메시 공격 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(AttackMontage);
	}

	// @Todo: 1인칭 공격 몽타주 재생
	UAnimInstance* FPSAnimInstance = FPSMeshComponent->GetAnimInstance();
	if (FPSAnimInstance)
	{
	}
}

void AHBCharacterPlayer::SetupHUDWidget(UHBUserHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		//@PHYTODO : 시간 경과에 따른 처리 진행
		AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
		if (GameState)
		{
			InHUDWidget->UpdatePhase(GameState->CurrentPhase);
			InHUDWidget->UpdateRemainingTime(GameState->RemainingTime);

			GameState->OnGamePhaseChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdatePhase);
			GameState->OnRemainingTimeChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateRemainingTime);
			GameState->OnTopDamagePlayersChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateCurrentFightInfo);
		}
	}
}

// @PHYTODO : 직업 분배 임시 확인용
void AHBCharacterPlayer::ServerRPCStart_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ServerRPCStart Call"));
	// 서버가 아니라면
	if (!HasAuthority())
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		// 현재 게임 모드 가져오기
		AHBVillageGameMode* VillageGameMode = Cast<AHBVillageGameMode>(GetWorld()->GetAuthGameMode());
		if (VillageGameMode)
		{
			// 게임이 진행중이면
			if (VillageGameMode->GetIsGamePlaying())
			{
				VillageGameMode->StopGame();
			}

			// 게임이 진행중이지 않으면
			else
			{
				VillageGameMode->StartGame();
			}
		}
		// UHBGameFlowSubsystem* GameFlowSubsystem = GameInstance->GetSubsystem<UHBGameFlowSubsystem>();
		// if (GameFlowSubsystem)
		// {
		// 	// 게임이 진행중이면
		// 	if (GameFlowSubsystem->GetIsGamePlaying())
		// 	{
		// 		// 게임 중지
		// 		GameInstance->GetSubsystem<UHBGameFlowSubsystem>()->StopGame();
		// 	}
		// 	// 게임이 진행중이지 않으면
		// 	else
		// 	{
		// 		// 게임 실행
		// 		GameInstance->GetSubsystem<UHBGameFlowSubsystem>()->StartGame();
		// 	}
		// }

		//OnRep_PlayerColor();
	}
}

void AHBCharacterPlayer::ClientRPCPlayAnimation_Implementation(AHBCharacterPlayer* CharacterToPlay)
{
	// CharacterToPlay 가 유효하면 공격 애니메이션 재생
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayAttackAnimation();
	}
}

void AHBCharacterPlayer::ServerRPCAttack_Implementation(float AttackStartTime)
{
	// 공격 중 공격을 막기 위해 플래그를 false로 변경
	bCanAttack = false;

	// 서버에서는 OnRep 호출이 안되니 명시적 호출
	// 일단 바뀌는 값이 없으니 Pass

	// 공격 시작 시간과 현재 서버 시간의 차이를 구함 (공격 사이의 딜레이)
	AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;

	// 해당 시간을 0초에서 실제 공격 애니메이션 길이보다 살짝 짧은 시간으로 범위 한정 (Clamp)
	AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.f, AttackTime - 0.1f);

	// 공격 끝나면 다시 공격 가능 여부 변경해주는 타이머 설정
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda(
			[&]()
			{
				bCanAttack = true;
			}
		), AttackTime - AttackTimeDifference, false, -1.f
	);

	// 마지막 공격 시작 시간을 이번 공격 시작 시간으로 변경
	LastAttackStartTime = AttackStartTime;

	// 공격 애니메이션 재생 (서버)
	PlayAttackAnimation();

	// 다른 클라이언트들의 애니메이션 재생 (공격 발생 클라 X, 서버 X)
	// 현재 월드에 있는 PlayerController 들을 받아옴
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		// PlayerController 가 유효하고 현재 사용중인 컨트롤러가 아니면
		if (PlayerController && GetController() != PlayerController)
		{
			// 현재 로컬 컨트롤러가 아니면
			if (!PlayerController->IsLocalController())
			{
				AHBCharacterPlayer* OtherPlayer = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					// 클라이언트의 PlayAnimation 을 재생
					OtherPlayer->ClientRPCPlayAnimation(this);
				}
			}
		}
	}
}

bool AHBCharacterPlayer::ServerRPCAttack_Validate(float AttackStartTime)
{
	// 공격 시간이 유효한지 체크
	if (LastAttackStartTime == 0.f)
	{
		return true;
	}

	// 현재 공격 시작 시간과 이전 공격 시작 시간이 애니메이션 길이보다 길어야 true 반환
	// 짧으면 악의적으로 공격을 빨리 하는거라 판단해서 false
	return (AttackStartTime - LastAttackStartTime) > AttackTime;
}

/* ================= Night Flow ================= */

void AHBCharacterPlayer::EnterHouse()
{
	if (!HasAuthority()) return;

	// 밤 상태에서만 의미 있음
	AHBMafiaGameState* GS = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!GS || !GS->IsNight())
	{
		return;
	}

	NightState = EPlayerNightState::InHouse;

	// 이번 밤에 한 번도 나간 적 없을 때만 회복 시작
	if (!bExitedHouseThisNight)
	{
		StartStaminaRecovery();
	}
}

void AHBCharacterPlayer::ExitHouse()
{
	if (!HasAuthority()) return;

	// 이미 밖이면 중복 처리 방지
	if (NightState == EPlayerNightState::Outside)
	{
		return;
	}

	AHBMafiaGameState* GS = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!GS || !GS->IsNight())
	{
		// 낮에는 Night Flow 상태를 건드리지 않음
		return;
	}

	NightState = EPlayerNightState::Outside;

	// 이번 밤에 외출 기록
	bExitedHouseThisNight = true;

	// 스태미나(사과) 1개 소모
	Stamina = FMath::Max(Stamina - 1, 0);

	// 한 번이라도 나갔으니 이번 밤 회복은 완전히 중단
	StopStaminaRecovery();

	UE_LOG(LogTemp, Warning,
		TEXT("[NightFlow] ExitHouse | Stamina: %d | ExitedThisNight: true"),
		Stamina);
}

void AHBCharacterPlayer::StartStaminaRecovery()
{
	if (!HasAuthority()) return;

	// 이미 회복 중이면 중복 실행 방지
	if (GetWorldTimerManager().IsTimerActive(StaminaRecoverTimerHandle))
	{
		return;
	}

	GetWorldTimerManager().SetTimer(
		StaminaRecoverTimerHandle,
		this,
		&AHBCharacterPlayer::RecoverStaminaTick,
		StaminaRecoverInterval,
		true
	);
}

void AHBCharacterPlayer::StopStaminaRecovery()
{
	if (!HasAuthority()) return;

	GetWorldTimerManager().ClearTimer(StaminaRecoverTimerHandle);
}

void AHBCharacterPlayer::RecoverStaminaTick()
{
	if (!HasAuthority()) return;

	// 이번 밤에 외출한 적 있으면 회복 금지
	if (bExitedHouseThisNight)
	{
		StopStaminaRecovery();
		return;
	}

	AHBMafiaGameState* GS = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!GS || !GS->IsNight())
	{
		StopStaminaRecovery();
		return;
	}

	// 집 안에 있을 때만 회복
	if (NightState != EPlayerNightState::InHouse)
	{
		StopStaminaRecovery();
		return;
	}

	const int32 OldStamina = Stamina;
	Stamina = FMath::Min(Stamina + StaminaRecoverAmount, MaxStamina);

	UE_LOG(LogTemp, Log,
		TEXT("[NightFlow] Recover %d -> %d"),
		OldStamina, Stamina);

	// 최대치면 회복 종료
	if (Stamina >= MaxStamina)
	{
		StopStaminaRecovery();
	}
}

void AHBCharacterPlayer::ResetNightState()
{
	if (!HasAuthority()) return;

	// 밤 시작 시 기본 상태 초기화
	bExitedHouseThisNight = false;
	NightState = EPlayerNightState::InHouse;

	// 혹시 남아있을지 모르는 타이머 정리
	StopStaminaRecovery();

	UE_LOG(LogTemp, Log,
		TEXT("[NightFlow] ResetNightState | Stamina: %d"),
		Stamina);
}
