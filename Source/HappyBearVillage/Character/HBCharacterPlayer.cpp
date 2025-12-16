// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterPlayer.h"

#include "EngineUtils.h"
#include "InteractiveToolManager.h"
#include "InputMappingContext.h"
#include "Animation/HBPlayerCharacterAnimInstance.h"
#include "GameFramework/GameStateBase.h"
#include "Interface/HBInteractableInterface.h"

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

	// InputMappingContext 설정
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

void AHBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	}
}

void AHBCharacterPlayer::Move(const FInputActionValue& Value)
{
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
	UE_LOG(LogTemp, Log, TEXT("Call Attack"));
	// 무기를 끼고있지 않으면 반환
	if (!bWeaponEquipped)
	{
		return;
	}
	
	if (bCanAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("Call Attack when bCanAttack is True"));
		
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
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Log, TEXT("Interaction 호출"));
		if (InteractionTarget != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("InteractionTarget 있음"));
			IHBInteractableInterface* InteractionActor = Cast<IHBInteractableInterface>(InteractionTarget);
			if (InteractionActor)
			{
				UE_LOG(LogTemp, Log, TEXT("InteractableInterface 도 있음"));
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

void AHBCharacterPlayer::InteractionTraceTick()
{
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
			UE_LOG(LogTemp, Log, TEXT("InteractionTarget = %s"), *HitActor->GetName());
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
}

void AHBCharacterPlayer::PlayAttackAnimation()
{
	UE_LOG(LogTemp, Log, TEXT("Call PlayAttackAnimation"));
	
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

void AHBCharacterPlayer::ClientRPCPlayAnimation_Implementation(AHBCharacterPlayer* CharacterToPlay)
{
	UE_LOG(LogTemp, Log, TEXT("Call ClientRPCPlayAnimation"));
	// CharacterToPlay 가 유효하면 공격 애니메이션 재생
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayAttackAnimation();
	}
}

void AHBCharacterPlayer::ServerRPCAttack_Implementation(float AttackStartTime)
{
	UE_LOG(LogTemp, Log, TEXT("Call ServerRPCAttack"));
	
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
