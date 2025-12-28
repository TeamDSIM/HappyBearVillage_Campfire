// Fill out your copyright notice in the Description page of Project Settings.


#include "HBCharacterMafiaAttackComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "Character/HBCharacterBase.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBCharacterRole.h"

UHBCharacterMafiaAttackComponent::UHBCharacterMafiaAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UHBCharacterMafiaAttackComponent::ToggleWeapon()
{
    // 1) Owner Pawn 확인
    APawn* PawnOwner = Cast<APawn>(GetOwner());
    if (!PawnOwner) return;

    // 2) 로컬 플레이어만 입력 처리
    if (!PawnOwner->IsLocallyControlled())
        return;

    // 3) UX용 로컬 체크 (서버에서도 다시 체크)
    if (!CanToggleWeapon())
        return;

    // 4) 서버에 실제 토글 요청
    Server_ToggleWeapon();
}

void UHBCharacterMafiaAttackComponent::Server_ToggleWeapon_Implementation()
{
    // 5) 서버에서 최종 검증 (보안)
    if (!CanToggleWeapon())
        return;

    AHBCharacterBase* Character = Cast<AHBCharacterBase>(GetOwner());
    if (!Character) return;

    if (Character->IsWeaponEquipped())
    {
        Character->UnEquipWeapon();
    }
    else
    {
        Character->EquipWeapon();
    }
}


bool UHBCharacterMafiaAttackComponent::CanToggleWeapon() const
{
    const AHBCharacterBase* Character = Cast<AHBCharacterBase>(GetOwner());
    if (!Character) return false;

    const UWorld* World = GetWorld();
    if (!World) return false;

    const AHBMafiaGameState* GS = World->GetGameState<AHBMafiaGameState>();
    if (!GS || !GS->IsNight())
        return false;

    const UHBPlayerStatComponent* StatComp = Character->GetStat();
    if (!StatComp) return false;

    const FHBCharacterRole& RoleData = StatComp->GetCharacterRole();
    if (RoleData.Role != ERoleType::MAFIA)
        return false;

    return true;
}