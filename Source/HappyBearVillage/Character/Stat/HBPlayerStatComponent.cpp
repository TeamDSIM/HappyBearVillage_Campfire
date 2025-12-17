// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stat/HBPlayerStatComponent.h"

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
	UE_LOG(LogTemp, Log, TEXT("%s Total Damage : %f"), *GetOwner()->GetName(), TotalTakenDamage);
	
	return TotalTakenDamage;
}
