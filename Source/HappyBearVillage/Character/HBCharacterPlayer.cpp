// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterPlayer.h"

#include "InteractiveToolManager.h"
#include "InputMappingContext.h"
#include "Animation/HBPlayerCharacterAnimInstance.h"

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

	// @Todo : 카메라 오프셋 구해서 머리에 붙이기
	FPSCameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, -75.0f, 510.0f), FRotator(0.0f, 90.0f, -90.0f));
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

	// 캐릭터 메시 설정
	GetMesh()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	FPSMeshComponent->SetAnimInstanceClass(AnimInstanceClass);
	
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
}

void AHBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Move);
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Attack);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::Interaction);
		
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AHBCharacterPlayer::MouseLook);
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

void AHBCharacterPlayer::Attack(const FInputActionValue& Value)
{
}

void AHBCharacterPlayer::Interaction(const FInputActionValue& Value)
{
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
