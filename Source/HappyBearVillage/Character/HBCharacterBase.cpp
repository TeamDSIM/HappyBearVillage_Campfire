// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterBase.h"

// Sets default values
AHBCharacterBase::AHBCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

