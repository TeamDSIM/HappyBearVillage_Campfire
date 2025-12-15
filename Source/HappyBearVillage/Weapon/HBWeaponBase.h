// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBWeaponBase.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHBWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Category=Weapon, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	// 상호작용할 아이템 박스에 할당된 방망이 번호
	// 실시간 생성을 최대한 줄이기 위해서 미리 상호작용 오브젝트에 방망이들 생성
	UPROPERTY(Category=Weapon, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	uint32 WeaponNum = 0;

public:
	bool IsEquipped() const;

	FORCEINLINE void SetWeaponNum(const uint32 InWeaponNum) { WeaponNum = InWeaponNum; }
	FORCEINLINE uint32 GetWeaponNum() const { return WeaponNum; }

};
