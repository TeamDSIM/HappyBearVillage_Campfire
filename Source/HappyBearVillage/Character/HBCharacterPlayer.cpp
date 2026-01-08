// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterPlayer.h"

#include "EngineUtils.h"
#include "HappyBearVillage.h"
#include "InputMappingContext.h"
#include "Animation/HBPlayerCharacterAnimInstance.h"
#include "Component/HBCharacterRagdollComponent.h"
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
#include "Character/Component/HBCharacterMafiaAttackComponent.h"
#include "Controller/HBPlayerController.h"
#include "Controller/HBVillagePlayerController.h"
#include "Controller/Component/HBMapWidgetComponent.h"

AHBCharacterPlayer::AHBCharacterPlayer()
{
	// FPS �޽� ����
	FPSMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMeshComponent->SetupAttachment(GetMesh());
	FPSMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FPSMeshComponent->SetOnlyOwnerSee(true);

	FPSMeshComponent->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	FPSMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 290.f));


	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	FPSMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	// FPS ī�޶� ����
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(FPSMeshComponent);

	// ī�޶� ������ ���ؼ� �Ӹ��� ���̱�
	FPSCameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, -75.0f, 560.0f), FRotator(0.0f, 90.0f, -90.0f));
	FPSCameraComponent->bUsePawnControlRotation = true;

	FPSCameraComponent->bEnableFirstPersonFieldOfView = true;
	FPSCameraComponent->bEnableFirstPersonScale = true;
	FPSCameraComponent->FirstPersonFieldOfView = FPSFieldOfView;
	FPSCameraComponent->FirstPersonScale = FPSViewScale;

	RagdollComponent = CreateDefaultSubobject<UHBCharacterRagdollComponent>(TEXT("Ragdoll"));

	// ���� �Է� ����
	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(
	//	TEXT("/Game/Character/Input/IMC_Player.IMC_Player"));
	//if (InputMappingContextRef.Succeeded())
	//{
	//	InputMappingContext = InputMappingContextRef.Object;
	//}

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

	// @PHYTODO : �ӽ� ���� �й�
	static ConstructorHelpers::FObjectFinder<UInputAction> StartActionRef(
		TEXT("/Game/Character/Input/Action/IA_Start.IA_Start"));
	if (StartActionRef.Succeeded())
	{
		StartAction = StartActionRef.Object;
	}

	// 직업 행동 IA
	static ConstructorHelpers::FObjectFinder<UInputAction> JobActionRef(
		TEXT("/Game/Character/Input/Action/IA_JobAction.IA_JobAction"));
	if (JobActionRef.Succeeded())
	{
		JobAction = JobActionRef.Object;
	}

	// �޽� ����
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

	static ConstructorHelpers::FObjectFinder<UInputAction> DanceActionRef(
		TEXT("/Game/Character/Input/Action/IA_Dance.IA_Dance"));
	if (DanceActionRef.Succeeded())
	{
		DanceAction = DanceActionRef.Object;
	}


	// ĳ���� �޽� ����
	GetMesh()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	FPSMeshComponent->SetAnimInstanceClass(AnimInstanceClass);

	// 1��Ī ���� �޽� ����
	FPSCurrentWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FPSWeapon"));
	FPSCurrentWeapon->SetupAttachment(FPSMeshComponent, TEXT("RightHandWeaponSocket"));
	FPSCurrentWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	FPSCurrentWeapon->SetIsReplicated(true);
	FPSCurrentWeapon->SetOnlyOwnerSee(true);
	FPSCurrentWeapon->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

	// ���� ���� ���� �ʱ�ȭ
	bCanAttack = true;

	//���� Ż/����
	MafiaAttackComp = CreateDefaultSubobject<UHBCharacterMafiaAttackComponent>(TEXT("MafiaAttackComp"));

	RenderColor = FLinearColor::Gray;
}

void AHBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	/* ================= Night Flow : Game Start Init ================= */
	if (HasAuthority())
	{
		// ���� ���� �ÿ��� ���(�����) �ʱ�ȭ
		Stamina = MaxStamina;
		bExitedHouseThisNight = false;
		NightState = EPlayerNightState::InHouse;
	}
	/* ================================================================ */

	DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("HitFlash"), 0.f);
		DynamicMaterial->SetVectorParameterValue(TEXT("HitTint"), FLinearColor(1.f, 0.f, 0.f, 1.f));
	}


	HandMeshDynamicMaterial = FPSMeshComponent->CreateDynamicMaterialInstance(0);

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

		// @PHYTODO : �ӽ� ���� �й�
		EnhancedInputComponent->BindAction(StartAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Start);

		// ���� Ż/��
		EnhancedInputComponent->BindAction(ToggleWeaponAction, ETriggerEvent::Triggered, this,
		                                   &AHBCharacterPlayer::ToggleWeapon);

		// 직업 행동 바인드
		EnhancedInputComponent->BindAction(JobAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::DoJobAction);

		// Dance Action Bind
		EnhancedInputComponent->BindAction(DanceAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Dance
		);
	}
}

void AHBCharacterPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBCharacterPlayer, PlayerColor)
	DOREPLIFETIME(AHBCharacterPlayer, RenderColor)

	// Night ���� ������Ƽ�� �����Ͽ� UI ����ȭ
	DOREPLIFETIME(AHBCharacterPlayer, Stamina);
	DOREPLIFETIME(AHBCharacterPlayer, NightState);
	DOREPLIFETIME(AHBCharacterPlayer, bExitedHouseThisNight);
	DOREPLIFETIME(AHBCharacterPlayer, bExitedPreviousNight);
}

float AHBCharacterPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HasAuthority() && ActualDamage > 0.f)
	{
		MulticastRPCApplyHitFlash(0.12f);
	}

	return ActualDamage;
}

void AHBCharacterPlayer::OnRep_Stamina()
{
	// RepNotify: Stamina�� �ٲ�� ���� �÷��̾� HUD�� �ݿ�
	if (!IsLocallyControlled())
	{
		return;
	}

	if (CachedHUDWidget)
	{
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("[NightFlow] OnRep_Stamina called but CachedHUDWidget is null. Stamina=%d"),
		       Stamina);
	}
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

	// ���� Phase �� Day(���� �̵� �� �ʱ�ȭ �ܰ�) �Ǵ� Discussion(��� �ܰ�) �̸� �̵� ����
	if (GameState->CurrentPhase == EGamePhase::Day || GameState->CurrentPhase == EGamePhase::Discussion)
	{
		return;
	}

	if (Stat->GetIsVoteTarget())
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

// ���� ����
void AHBCharacterPlayer::Attack()
{
	// ���⸦ �������� ������ ��ȯ
	if (!bWeaponEquipped)
	{
		return;
	}

	if (Stat->GetIsVoteTarget())
	{
		return;
	}

	if (bCanAttack)
	{
		if (!HasAuthority())
		{
			// ���� �� ���� ���ϰ� ����
			bCanAttack = false;

			// ���� �ִϸ��̼� ���� �� ����� ��������Ʈ �Լ� ���ε�
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

			// Ŭ���̾�Ʈ�� �ִϸ��̼� ���
			PlayAttackAnimation();
		}

		// ServerRPC ���� ����
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

	// ���� Phase �� ���� Phase �� �ƴϸ� ��ȣ�ۿ� ���� ���ϵ��� ����
	if (GameState->CurrentPhase != EGamePhase::Fight && GameState->CurrentPhase != EGamePhase::Lobby)
	{
		return;
	}
	if (Stat->GetIsVoteTarget())
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
	// �����̸� ���� �й�
	if (!HasAuthority())
	{
		return;
	}

	ServerRPCStart();
}

void AHBCharacterPlayer::ToggleWeapon()
{
	if (MafiaAttackComp)
		MafiaAttackComp->ToggleWeapon();
}

void AHBCharacterPlayer::DoJobAction()
{
	if (GetJobComponent())
	{
		GetJobComponent()->Action();
		// 각 직업의 컴포넌트에서 필요한 부분 구현
		// 충돌 판정 같은게 필요하다면 공격판정을 참고하여 ServerRPC 를 돌려야 할듯 함
	}
}

//Marker 색상 동기화
void AHBCharacterPlayer::OnRep_PlayerColor()
{
	SetFPSMeshColor();

	AHBVillagePlayerController* HBPlayerController = Cast<AHBVillagePlayerController>(GetController());
	if (!HBPlayerController) return;

	UHBMapWidgetComponent* MapWidgetComponent = HBPlayerController->GetComponentByClass<UHBMapWidgetComponent>();
	if (!MapWidgetComponent) return;

	MapWidgetComponent->SetOwnMarkColor(PlayerColor);
}

void AHBCharacterPlayer::OnRep_RenderColor()
{
	ChangeCharacterMaterialColor(RenderColor);
}

//랜덤 색깔이 아니라, 색깔 적용 함수
void AHBCharacterPlayer::SetFPSMeshColor()
{
	// DynamicMaterial 없으면 생성
	if (!DynamicMaterial && GetMesh())
	{
		DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
	}

	if (!HandMeshDynamicMaterial && FPSMeshComponent)
	{
		HandMeshDynamicMaterial = FPSMeshComponent->CreateDynamicMaterialInstance(0);
	}

	if (HandMeshDynamicMaterial)
	{
		HandMeshDynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), PlayerColor);
	}
}

void AHBCharacterPlayer::ResetBaseColor()
{
	// DynamicMaterial ����
	if (!DynamicMaterial && GetMesh())
	{
		DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0);
	}

	if (!HandMeshDynamicMaterial && FPSMeshComponent)
	{
		HandMeshDynamicMaterial = FPSMeshComponent->CreateDynamicMaterialInstance(0);
	}

	if (DynamicMaterial)
	{
		// �����϶��� ���� ���� ���� ����
		if (HasAuthority())
		{
			AHBPlayerState* HBPlayerState = GetController()->GetPlayerState<AHBPlayerState>();
			if (HBPlayerState)
			{
				HBPlayerState->SetPlayerColor(FLinearColor::Gray);
			}
			//PlayerColor = FLinearColor::White;
		}

		// CharacterBaseColor ������ RandomColor ���� �ο�
		DynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), PlayerColor);
	}

	if (HandMeshDynamicMaterial)
	{
		HandMeshDynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), PlayerColor);
	}
}

void AHBCharacterPlayer::ChangeCharacterMaterialColor(FLinearColor NewColor)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), NewColor);
	}
}

void AHBCharacterPlayer::ApplyHitFlash()
{
	HitFlashElapsed = 0.f;
	GetWorldTimerManager().ClearTimer(HitFlashTimerHandle);

	// 효과 키기
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("HitFlash"), 1.f);
	}

	// 타이머 후 효과 끄기
	GetWorldTimerManager().SetTimer(
		HitFlashTimerHandle,
		this,
		&AHBCharacterPlayer::UpdateHitFlash,
		0.016f,
		true
	);
}

void AHBCharacterPlayer::UpdateHitFlash()
{
	HitFlashElapsed += 0.016f;
	
	float Alpha = HitFlashElapsed / HitFlashDuration;
	float FlashValue = FMath::Lerp(1.f, 0.f, Alpha);
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("HitFlash"), FlashValue);
	}
	
	if (Alpha >= 1.f)
	{
		GetWorldTimerManager().ClearTimer(HitFlashTimerHandle);
	}
}

void AHBCharacterPlayer::MulticastRPCApplyHitFlash_Implementation(float Duration)
{
	ApplyHitFlash();
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

	// ���� Phase �� ���� Phase �� �ƴϸ� ��ȣ�ۿ����� �����Ƿ� �Ǻ� ���� X
	if (GameState->CurrentPhase != EGamePhase::Fight && GameState->CurrentPhase != EGamePhase::Lobby)
	{
		return;
	}

	const FRotator CurrentRotation = GetControlRotation();

	// �ü� ��ȭ üũ �� ����ġ�� ���� �ʾ����� ��ŵ
	if (CurrentRotation.Equals(LastControlRotation, ViewAngleThreshold))
	{
		return;
	}

	LastControlRotation = CurrentRotation;

	FVector Start = FPSCameraComponent->GetComponentLocation();
	FVector End = Start + (FPSCameraComponent->GetForwardVector() * InteractionDistance);

	// Ʈ���̽� ����
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
	// ���� ���� ���ӵ� �÷��̾� Ŭ���̾�Ʈ�� ���� ���������� Ȯ��
	if (IsLocallyControlled())
	{
		// �浹 ���� ���ϱ�
		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		// ���� ���� ���� ���ϱ� (��Ÿ�, ����, ������ ��)
		float AttackRange = Stat->GetBaseStat().AttackRange;
		float AttackRadius = Stat->GetBaseStat().AttackRadius;
		float AttackDamage = Stat->GetBaseStat().AttackDamage;

		// �÷��̾� Forward ����, ���� ����/�� ���� ���� ���ϱ�
		FVector Forward = GetActorForwardVector();
		FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
		FVector End = Start + GetActorForwardVector() * AttackRange;

		// Sweep ���� �浹
		// @Todo: single ����, multi ���� (�ϴ� single�� ����)
		bool HitDetected = GetWorld()->SweepSingleByChannel(
			OutHitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(AttackRadius),
			Params
		);

		// �浹 üũ ���� �ð� ����
		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		// ������ �ƴ϶��
		if (!HasAuthority())
		{
			// �浹 �ߴٸ�
			if (HitDetected)
			{
				// ����RPC �� �浹 ��Ƽ���� ����
				ServerRPCNotifyHit(OutHitResult, HitCheckTime);
			}

			// �浹 ���ߴٸ�
			else
			{
				// ����RPC �� �̽� ��Ƽ���� ����
				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
			}
		}

		// �������
		else
		{
			// ���� ���� ����� ǥ��
			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			DrawDebugAttackRange(DebugColor, Start, End, Forward);

			// �浹 �ߴٸ�
			if (HitDetected)
			{
				// ������ ����
				AttackHitConfirm(OutHitResult.GetActor());
			}
		}
	}
}

void AHBCharacterPlayer::AttackHitConfirm(AActor* HitActor)
{
	// �������
	if (HasAuthority())
	{
		// ���� ������ ��������
		float AttackDamage = Stat->GetBaseStat().AttackDamage;

		// ��?H�� ��󿡰� TakeDamage �� ������ ����
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);

		// 직업의 공격 함수 호출 (피격 시 적용되는것만 있으니 HitConfirm 에서 호출)
		if (GetJobComponent())
		{
			UE_LOG(LogTemp, Log, TEXT("JobComponent Attack"));
			GetJobComponent()->Attack(HitActor);
		}

		AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
		if (GameState && GameState->CurrentPhase == EGamePhase::Fight)
		{
			UHBGameVoteSubsystem* VoteSubsystem = GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
			if (VoteSubsystem)
			{
				// @PHYTODO : Top3 ���� ����
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
	// �浹�� ���� ���� ������
	AActor* HitActor = HitResult.GetActor();

	// ���� ���� �����ϸ�
	if (HitActor)
	{
		// �浹 �߻� ��ġ, �浹 �߻��� ������ �ٿ�� �ڽ�, �ٿ�� �ڽ��� ��� ����
		FVector HitLocation = HitResult.Location;
		FBox HitBox = HitActor->GetComponentsBoundingBox();
		FVector ActorBoxCenter = HitBox.GetCenter();

		// �浹 �߻� ��ġ�� �ٿ�� �ڽ��� �߰� ������ �浹 üũ �ּ� �Ÿ����� ������
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			// �浹 �� �̺�Ʈ ����
			AttackHitConfirm(HitActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HitTest Reject"));
		}

		// �浹 �� Notify �̴� ����� �÷��� Green �� �־ ������� ��
		DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd,
		                     HitActor->GetActorForwardVector());
	}
}

bool AHBCharacterPlayer::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitCheckTime)
{
	// ������ ���� ���� �ð��� 0 (ó�� ����)
	if (LastAttackStartTime == 0.f)
	{
		return true;
	}
	// �浹 üũ �ð��̶� ������ ������ �ð��� ���̰� �ּ� üũ ���ݺ��� Ŀ�� üũ ����
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
}

void AHBCharacterPlayer::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart,
                                                            FVector_NetQuantize TraceEnd,
                                                            FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	// �浹 ���� �� ���� ���� ����� �׸���
	DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}

bool AHBCharacterPlayer::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd,
                                                      FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	// ������ ���� ���� �ð��� 0 (ó�� ����)
	if (LastAttackStartTime == 0.f)
	{
		return true;
	}
	// �浹 üũ �ð��̶� ������ ������ �ð��� ���̰� �ּ� üũ ���ݺ��� Ŀ�� üũ ����
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
}

void AHBCharacterPlayer::PlayAttackAnimation()
{
	// ĳ���� �޽� ���� ��Ÿ�� ���
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(AttackMontage);
	}

	// @Todo: 1��Ī ���� ��Ÿ�� ���
	UAnimInstance* FPSAnimInstance = FPSMeshComponent->GetAnimInstance();
	if (FPSAnimInstance)
	{
		FPSAnimInstance->StopAllMontages(0.f);
		FPSAnimInstance->Montage_Play(AttackMontage);
	}
}

void AHBCharacterPlayer::ClientRPCPlayDance_Implementation(AHBCharacterPlayer* CharacterToPlay, int32 MontageIndex,
                                                           float StartTime)
{
	// 서버가 클라이언트에게 춤 재생을 지시할 때 클라이언트에서 실행됩니다.
	// attack 흐름을 참고하여 몽타주 유효성 검사 후 재생합니다.

	// 유효한 인덱스인지 확인
	if (!DanceMontages.IsValidIndex(MontageIndex) || !DanceMontages[MontageIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientRPCPlayDance: Invalid montage index %d"), MontageIndex);
		return;
	}

	// 상태 저장 (중복 재생 방지 등 로직에서 필요할 수 있음)
	LastDanceIndex = MontageIndex;

	// 메시에 연결된 AnimInstance에서 몽타주 재생
	if (CharacterToPlay)
	{
		//	// 필요 시 기존 몽타주를 중단 (공격과 동일한 처리 방식)
		//	AnimInstance->StopAllMontages(0.f);

		//	// 1인칭에서는 춤 애니메이션을 재생하지 않도록 기존 설계에 맞춰
		//	// Mesh(3인칭)만 재생. StartTime을 사용해 서버와의 싱크 보정.
		//	AnimInstance->Montage_Play(DanceMontages[MontageIndex], 1.f, EMontagePlayReturnType::MontageLength, StartTime);
		//
		CharacterToPlay->PlayDanceAnimation(MontageIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ClientRPCPlayDance: No AnimInstance found for player"));
	}
}

void AHBCharacterPlayer::ServerRPCDance_Implementation(int32 MontageIndex, float StartTime)
{
	// 서버에서만 실행되어야 함
	if (!HasAuthority())
	{
		return;
	}

	// 유효성 검사
	if (!DanceMontages.IsValidIndex(MontageIndex) || !DanceMontages[MontageIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRPCDance: Invalid montage index %d"), MontageIndex);
		return;
	}

	// 상태 저장 (중복 재생 방지용 등)
	LastDanceIndex = MontageIndex;

	// 무기 숨기기(서버 측 로컬 처리가 필요하면 실행)
	// 서버의 Pawn(호스트 클라이언트인 경우)은 직접 처리
	SetWeaponVisibleForDance(false);

	// 모든 플레이어에게 춤 재생을 지시
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	// 서버 자신은 직접 재생
	PlayDanceAnimation(MontageIndex);

	// 각 클라이언트의 소유 액터에 RPC를 호출하되,
	// RPC 파라미터로는 '춤을 실제로 재생할 액터'인 this(서버의 춤추는 캐릭터)를 전달
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			// 서버 로컬 컨트롤러(호스트 자체)는 건너뜀
			if (!PlayerController->IsLocalController())
			{
				AHBCharacterPlayer* OtherPlayer = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					// OtherPlayer(클라이언트가 소유한 액터)에서 RPC 호출하여
					// 해당 클라이언트 측에서 this 캐릭터의 춤을 재생하게 함
					OtherPlayer->ClientRPCPlayDance(this, MontageIndex, StartTime);
				}
			}
		}
	}
}

void AHBCharacterPlayer::PlayDanceAnimation(int32 MontageIndex)
{
	// 유효성 검사
	if (!DanceMontages.IsValidIndex(MontageIndex) || !DanceMontages[MontageIndex])
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayDanceAnimation: Invalid montage index %d"), MontageIndex);
		return;
	}

	// 상태 저장
	LastDanceIndex = MontageIndex;

	// 3인칭 메시(겉보기)에서 몽타주 재생 (기존 설계: 1인칭은 재생하지 않음)
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		UE_LOG(LogTemp, Log, TEXT("PlayDanceAnimation: Playing dance montage index %d"), MontageIndex);
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(DanceMontages[MontageIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayDanceAnimation: No AnimInstance on Mesh"));
	}

	// 춤 재생 중에는 무기 숨기기
	SetWeaponVisibleForDance(false);

	// 몽타주 길이만큼 대기 후 무기 복구
	float MontageLength = DanceMontages[MontageIndex]->GetPlayLength();
	if (MontageLength <= 0.f)
	{
		// 안전한 기본값
		MontageLength = 1.0f;
	}

	FTimerHandle RestoreHandle;
	GetWorldTimerManager().SetTimer(
		RestoreHandle,
		FTimerDelegate::CreateLambda([this]()
		{
			SetWeaponVisibleForDance(true);
		}),
		MontageLength,
		false
	);
}

void AHBCharacterPlayer::SetWeaponVisibleForDance(bool bVisible)
{
	// 3인칭(Third-person) 무기 컴포넌트 처리 (AHBCharacterBase::CurrentWeapon 사용)
	if (CurrentWeapon)
	{
		// 가시성 및 히든 플래그 동기화
		CurrentWeapon->SetVisibility(bVisible, true);
		CurrentWeapon->SetHiddenInGame(!bVisible, true);
	}

	// 1인칭(First-person) 무기 처리
	if (FPSCurrentWeapon)
	{
		// FPS 무기는 원래 OnlyOwnerSee 설정이 있을 수 있으므로 단순히 히든/가시성 토글
		FPSCurrentWeapon->SetVisibility(bVisible, true);
		FPSCurrentWeapon->SetHiddenInGame(!bVisible, true);
	}

	// 필요 시 추가적으로 충돌 비활성화/활성화 등도 여기서 처리할 수 있음
}

void AHBCharacterPlayer::SetupHUDWidget(UHBUserHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		//@PHYTODO : �ð� ����� ���� ó�� ����
		AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
		if (GameState)
		{
			InHUDWidget->UpdatePhase(GameState->CurrentPhase);
			InHUDWidget->UpdateRemainingTime(GameState->RemainingTime);
			InHUDWidget->UpdateDate(GameState->Date);

			GameState->OnGamePhaseChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdatePhase);
			GameState->OnRemainingTimeChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateRemainingTime);
			GameState->OnDateChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateDate);
			GameState->OnTopDamagePlayersChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateCurrentFightInfo);
			GameState->OnTargetVoteNumChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateVoteNum);
			GameState->OnGameEndChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateGameEnd);

			GameState->OnFadeAnimationPlay.AddUObject(InHUDWidget, &UHBUserHUDWidget::PlayFadeAnimation);
			OnStaminaChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdateStamina);
			OnPoliceEffectChanged.AddUObject(InHUDWidget, &UHBUserHUDWidget::UpdatePoliceNotice);
		}

		// HUD�� Stamina ����: �ʱⰪ ���� �� ĳ�� (���� Ŭ���̾�Ʈ������)
		if (IsLocallyControlled())
		{
			CachedHUDWidget = InHUDWidget;
		}
	}
}

// 직업 설정
void AHBCharacterPlayer::AssignJob(TSubclassOf<UHBJobBaseComponent> JobClass)
{
	// 서버에서만 실행
	if (!HasAuthority())
	{
		return;
	}

	if (JobComponent)
	{
		JobComponent->DestroyComponent();
		JobComponent = nullptr;
	}

	JobComponent = NewObject<UHBJobBaseComponent>(this, JobClass);

	if (JobComponent)
	{
		JobComponent->RegisterComponent();
	}
}

void AHBCharacterPlayer::ClientRPCSetPoliceNotice_Implementation(bool bIsSealed)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	OnPoliceEffectChanged.Broadcast(bIsSealed);
}

// @PHYTODO : ���� �й� �ӽ� Ȯ�ο�
void AHBCharacterPlayer::ServerRPCStart_Implementation()
{
	// ������ �ƴ϶��
	if (!HasAuthority())
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		// ���� ���� ��� ��������
		AHBVillageGameMode* VillageGameMode = Cast<AHBVillageGameMode>(GetWorld()->GetAuthGameMode());
		if (VillageGameMode)
		{
			// ������ �������̸�
			if (VillageGameMode->GetIsGamePlaying())
			{
				VillageGameMode->CheatPhaseChange();
				//VillageGameMode->StopGame();
			}

			// ������ ���������� ������
			else
			{
				VillageGameMode->StartGame();
			}
		}
	}
}

void AHBCharacterPlayer::ClientRPCPlayAnimation_Implementation(AHBCharacterPlayer* CharacterToPlay)
{
	// CharacterToPlay �� ��ȿ�ϸ� ���� �ִϸ��̼� ���
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayAttackAnimation();
	}
}

void AHBCharacterPlayer::ServerRPCAttack_Implementation(float AttackStartTime)
{
	// ���� �� ������ ���� ���� �÷��׸� false�� ����
	bCanAttack = false;

	// ���� ���� �ð��� ���� ���� �ð��� ���̸� ���� (���� ������ ������)
	AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;

	// �ش� �ð��� 0�ʿ��� ���� ���� �ִϸ��̼� ���̺��� ��¦ ª�� �ð����� ���� ���� (Clamp)
	AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.f, AttackTime - 0.1f);

	// ���� ������ �ٽ� ���� ���� ���� �������ִ� Ÿ�̸� ����
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

	// ������ ���� ���� �ð��� �̹� ���� ���� �ð����� ����
	LastAttackStartTime = AttackStartTime;

	// ���� �ִϸ��̼� ��� (����)
	PlayAttackAnimation();

	// �ٸ� Ŭ���̾�Ʈ���� �ִϸ��̼� ��� (���� �߻� Ŭ�� X, ���� X)
	// ���� ���忡 �ִ� PlayerController ���� �޾ƿ�
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		// PlayerController �� ��ȿ�ϰ� ���� ������� ��Ʈ�ѷ��� �ƴϸ�
		if (PlayerController && GetController() != PlayerController)
		{
			// ���� ���� ��Ʈ�ѷ��� �ƴϸ�
			if (!PlayerController->IsLocalController())
			{
				AHBCharacterPlayer* OtherPlayer = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer && OtherPlayer != this)
				{
					// Ŭ���̾�Ʈ�� PlayAnimation �� ���
					OtherPlayer->ClientRPCPlayAnimation(this);
				}
			}
		}
	}
}

bool AHBCharacterPlayer::ServerRPCAttack_Validate(float AttackStartTime)
{
	// ���� �ð��� ��ȿ���� üũ
	if (LastAttackStartTime == 0.f)
	{
		return true;
	}

	// ���� ���� ���� �ð��� ���� ���� ���� �ð��� �ִϸ��̼� ���̺��� ���� true ��ȯ
	// ª���� ���������� ������ ���� �ϴ°Ŷ� �Ǵ��ؼ� false
	return (AttackStartTime - LastAttackStartTime) > AttackTime;
}

/* ================= Night Flow ================= */

void AHBCharacterPlayer::EnterHouse()
{
	if (!HasAuthority()) return;

	// �� ���¿����� �ǹ� ����
	AHBMafiaGameState* GS = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!GS || !GS->IsNight())
	{
		return;
	}

	NightState = EPlayerNightState::InHouse;

	// �̹� �㿡 �� ���� ���� �� ���� ���� ȸ�� ����
	if (!bExitedHouseThisNight)
	{
		StartStaminaRecovery();
	}
}

void AHBCharacterPlayer::ExitHouse()
{
	UE_LOG(LogTemp, Log, TEXT("ExitHouse called"));

	if (!HasAuthority()) return;

	// �̹� ���̸� �ߺ� ó�� ����
	if (NightState == EPlayerNightState::Outside)
	{
		return;
	}

	AHBMafiaGameState* GS = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!GS || !GS->IsNight())
	{
		// ������ Night Flow ���¸� �ǵ帮�� ����
		return;
	}

	NightState = EPlayerNightState::Outside;

	// �̹� �㿡 ���� ���
	bExitedHouseThisNight = true;

	// ���¹̳�(���) 1�� �Ҹ� ? ��� �Ҹ� ��Ģ
	Stamina = FMath::Max(Stamina - 1, 0);
	OnStaminaChanged.Broadcast(Stamina);

	// �� ���̶� �������� �̹� �� ȸ���� ������ �ߴ�
	StopStaminaRecovery();

	UE_LOG(LogTemp, Warning,
	       TEXT("[NightFlow] ExitHouse | Stamina: %d | ExitedThisNight: true"),
	       Stamina);
}

void AHBCharacterPlayer::StartStaminaRecovery()
{
	if (!HasAuthority()) return;

	// �̹� ȸ�� ���̸� �ߺ� ���� ����
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

	// �̹� �㿡 ������ �� ������ ȸ�� ����
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

	// �� �ȿ� ���� ���� ȸ��
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

	// �ִ�ġ�� ȸ�� ����
	if (Stamina >= MaxStamina)
	{
		StopStaminaRecovery();
	}
}

void AHBCharacterPlayer::ResetNightState()
{
	if (!HasAuthority()) return;

	// ���� ���� ���� ���θ� ���(���� ���� ����)
	bExitedPreviousNight = bExitedHouseThisNight;

	// �� ���� �� �⺻ ���� �ʱ�ȭ (�̹� ���� ���� ���� ����)
	bExitedHouseThisNight = false;
	NightState = EPlayerNightState::InHouse;

	// Ȥ�� ���������� �𸣴� Ÿ�̸� ����
	StopStaminaRecovery();

	UE_LOG(LogTemp, Log,
	       TEXT("[NightFlow] ResetNightState | Stamina: %d | PrevExited: %s"),
	       Stamina, bExitedPreviousNight ? TEXT("true") : TEXT("false"));
}

void AHBCharacterPlayer::ProcessNightEnd()
{
	if (!HasAuthority()) return;

	// ��Ģ: "����(�ٷ� ���� ��)�� ����������, �� ���� ��(���� ��) ���� �������� ���� ��� ���¹̳� +1 ȸ��"
	// ���⼭�� '���� ���� ��'(-> �� ���� ��) ȣ��Ǿ�, bExitedPreviousNight �� �ٷ� ������ ��ϵ� ��,
	// bExitedHouseThisNight �� ���� ���� ��(�� ��)�� �����ߴ��� ����.
	if (bExitedPreviousNight && !bExitedHouseThisNight)
	{
		const int32 Old = Stamina;
		Stamina = FMath::Min(Stamina + 1, MaxStamina);
		UE_LOG(LogTemp, Log,
		       TEXT("[NightFlow] ProcessNightEnd: Recovered %d -> %d (PrevExited true, ThisNight no exit)"),
		       Old, Stamina);
	}

	// ���� ����Ŭ�� ���� ���� �� �÷��� ���� (���� bExitedPreviousNight�� ��� ���� ���� ���� ���η� ����)
	bExitedPreviousNight = bExitedHouseThisNight;
}

void AHBCharacterPlayer::Dance()
{
	if (DanceMontages.Num() == 0)
		return;

	int32 Index = 0;

	// 랜덤 선택 (연속 중복 방지)
	if (DanceMontages.Num() == 1)
	{
		Index = 0;
	}
	else
	{
		do
		{
			Index = FMath::RandRange(0, DanceMontages.Num() - 1);
		}
		while (Index == LastDanceIndex);
	}

	LastDanceIndex = Index;

	// 로컬 즉시 재생
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Dance: Playing dance montage index %d locally"), Index);
		PlayDanceAnimation(Index);
	}

	// 서버 시간 안전 처리
	UWorld* World = GetWorld();
	AGameStateBase* GS = World ? World->GetGameState() : nullptr;
	const float StartTime = GS ? GS->GetServerWorldTimeSeconds() : (World ? World->GetTimeSeconds() : 0.f);

	// 서버에 알림
	ServerRPCDance(Index, StartTime);
}
