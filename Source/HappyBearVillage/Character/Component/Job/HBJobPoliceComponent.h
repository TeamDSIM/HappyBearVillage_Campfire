// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobPoliceComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobPoliceComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobPoliceComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void NightPhaseBegin() override;
	virtual void NightPhaseEnd() override;
	
	virtual void Action() override;

	UFUNCTION()
	void OnRep_IsRemainAction();
	
protected:
	UFUNCTION(Server, Reliable)
	void ServerRPCAction();

	void BindSealEvent();

private:
	
	// 스킬 봉인, 무기 해제
	UFUNCTION()
	void SealPlayer(AActor* InActor);

	// 플레이어 행동 봉인 해제
	UFUNCTION()
	void UnsealPlayer(AActor* InActor);

	// 바인딩할 함수 델리게이트 핸들 (초기화 용)
	FDelegateHandle EnterDelegateHandle;
	FDelegateHandle ExitDelegateHandle;

	// 직업용으로 바인딩 된 함수만 해제하기 위해서 집값을 갖고있어야함
	UPROPERTY(Replicated)
	TObjectPtr<AHBHouse> HBHouse = nullptr;
	
	// 밤에 능력 사용 가능 여부
	// true 면 사용 가능, false 면 불가
	UPROPERTY(ReplicatedUsing = OnRep_IsRemainAction)
	uint8 bIsRemainAction : 1;
};
