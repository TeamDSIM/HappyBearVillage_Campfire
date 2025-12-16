// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/HBAnimNotify_AttackHitCheck.h"

#include "Interface/HBAttackAnimationInterface.h"

void UHBAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IHBAttackAnimationInterface* AttackPawn = Cast<IHBAttackAnimationInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
