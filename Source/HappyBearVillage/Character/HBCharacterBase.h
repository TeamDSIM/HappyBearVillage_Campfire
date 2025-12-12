// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HBCharacterBase.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHBCharacterBase();

	// 나중에 델리게이트 함수 바인딩용
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
protected:
	// ACharacter 에 있는 것
	// Mesh, CharacterMovement, CapsuleComponent, Arrow

	// @Todo:
	// 추가해야할 것
	// 스탯, 애니메이션, 공격, 기절, 무기 장착
	
};
