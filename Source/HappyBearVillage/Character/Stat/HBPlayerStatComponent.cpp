// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stat/HBPlayerStatComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHBPlayerStatComponent::UHBPlayerStatComponent()
{
	bWantsInitializeComponent = true;
	
	SetIsReplicated(true);
}

void UHBPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called when the game starts
void UHBPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UHBPlayerStatComponent::ApplyDamage(float InDamageAmount)
{
	TotalTakenDamage += InDamageAmount;
	OnTotalTakenDamageChanged.Broadcast(TotalTakenDamage);
	
	return TotalTakenDamage;
}

void UHBPlayerStatComponent::InitCharacterRole()
{
	// 랜덤 직업 설정
	CharacterRole.InitRole();

	// 바뀐 직업 서버 반영
	OnRep_CharacterRole();
}

void UHBPlayerStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBPlayerStatComponent, TotalTakenDamage);
	DOREPLIFETIME(UHBPlayerStatComponent, CharacterRole);
}

void UHBPlayerStatComponent::OnRep_TotalTakenDamage()
{
	OnTotalTakenDamageChanged.Broadcast(TotalTakenDamage);
}

void UHBPlayerStatComponent::OnRep_CharacterRole()
{
	// UI 갱신 시켜야 함
	OnPlayerRoleChanged.Broadcast(CharacterRole.Role);
	OnPlayerJobChanged.Broadcast(CharacterRole.Job);
	
}
