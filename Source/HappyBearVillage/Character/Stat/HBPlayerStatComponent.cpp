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
	UE_LOG(LogTemp, Log, TEXT("%s Total Damage : %f"), *GetOwner()->GetName(), TotalTakenDamage);
	
	return TotalTakenDamage;
}

void UHBPlayerStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBPlayerStatComponent, TotalTakenDamage);
}

void UHBPlayerStatComponent::OnRep_TotalTakenDamage()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_TotalTakenDamage Call"));
	
	OnTotalTakenDamageChanged.Broadcast(TotalTakenDamage);
}
