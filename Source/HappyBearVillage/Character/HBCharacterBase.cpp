// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterBase.h"

#include "Net/UnrealNetwork.h"
#include "Stat/HBPlayerStatComponent.h"
#include "UI/HBTotalDamageWidget.h"
#include "UI/HBWidgetComponent.h"
#include "Weapon/HBWeaponBase.h"

// Sets default values
AHBCharacterBase::AHBCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	CurrentWeapon->SetupAttachment(GetMesh(), TEXT("RightHandWeaponSocket"));
	CurrentWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	CurrentWeapon->SetIsReplicated(true);
	CurrentWeapon->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(
		TEXT("/Game/Assets/Fab/Baseball_Bat/baseball_bat/StaticMeshes/baseball_bat.baseball_bat"));
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh = WeaponMeshRef.Object;
	}

	// Stat 컴포넌트
	Stat = CreateDefaultSubobject<UHBPlayerStatComponent>(TEXT("Stat"));

	// TotalDamage 위젯 컴포넌트
	TotalDamageWidget = CreateDefaultSubobject<UHBWidgetComponent>(TEXT("TotalDamageWidget"));
	TotalDamageWidget->SetupAttachment(GetMesh());
	TotalDamageWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> TotalDamageWidgetRef(
		TEXT("/Game/Personal/PARK_H_Y/UI/WBP_TotalDamage.WBP_TotalDamage_C"));
	if (TotalDamageWidgetRef.Class)
	{
		UE_LOG(LogTemp, Log, TEXT("Total Widget component Init"));
		
		TotalDamageWidget->SetWidgetClass(TotalDamageWidgetRef.Class);
		TotalDamageWidget->SetWidgetSpace(EWidgetSpace::Screen);
		TotalDamageWidget->SetDrawSize(FVector2D(150.f, 15.f));
		TotalDamageWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AHBCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AHBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHBCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHBCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHBCharacterBase::SetUpCharacterWidget(class UHBUserWidget* InUserWidget)
{
	UHBTotalDamageWidget* Widget = Cast<UHBTotalDamageWidget>(InUserWidget);
	if (Widget)
	{
		UE_LOG(LogTemp, Log, TEXT("Setup Character Widget Call"));
		Widget->UpdateTotalDamage(Stat->GetTotalTakenDamage());
		Stat->OnTotalTakenDamageChanged.AddUObject(Widget, &UHBTotalDamageWidget::UpdateTotalDamage);
	}
}

void AHBCharacterBase::ServerRPCSetEquipped_Implementation(bool bEquipped)
{
	bWeaponEquipped = bEquipped;
	//OnRep_WeaponEquipped();

	SetWeaponMesh();
}

void AHBCharacterBase::EquipWeapon()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("EquipWeapon 호출"));
		
		bWeaponEquipped = true;
		OnRep_WeaponEquipped();
	}
	else
	{
		ServerRPCSetEquipped(true);
	}
}

void AHBCharacterBase::UnEquipWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("UnEquipWeapon 호출"));
	if (CurrentWeapon->GetStaticMesh() != nullptr)
	{
		bWeaponEquipped = false;
		CurrentWeapon->SetStaticMesh(nullptr);
	}
}

void AHBCharacterBase::SetWeaponMesh()
{
	if (bWeaponEquipped == true)
	{
		if (WeaponMesh != nullptr)
		{
			if (CurrentWeapon->GetStaticMesh() == nullptr)
			{
				CurrentWeapon->SetStaticMesh(WeaponMesh);
			}
		}
	}
	
	else
	{
		if (CurrentWeapon->GetStaticMesh() != nullptr)
		{
			CurrentWeapon->SetStaticMesh(nullptr);
		}
	}
}

void AHBCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBCharacterBase, bWeaponEquipped);
}

void AHBCharacterBase::OnRep_WeaponEquipped()
{
	GetWorldTimerManager().SetTimerForNextTick(this, &AHBCharacterBase::SetWeaponMesh);
	//SetWeaponMesh();
}

void AHBCharacterBase::AttackHitCheck()
{
}

float AHBCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("Call CharacterBase TakeDamage / Damage : %f"), DamageAmount);
	Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

