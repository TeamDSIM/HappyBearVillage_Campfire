// Fill out your copyright notice in the Description page of Project Settings.


#include "HBCharacterMafiaAttackComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "Character/HBCharacterBase.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBCharacterRole.h"
#include "Net/UnrealNetwork.h"

UHBCharacterMafiaAttackComponent::UHBCharacterMafiaAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    bIsCanActiveEquip = true;
}

void UHBCharacterMafiaAttackComponent::ToggleWeapon()
{
    // 1) Owner Pawn Ȯ��
    APawn* PawnOwner = Cast<APawn>(GetOwner());
    if (!PawnOwner) return;

    // 2) ���� �÷��̾ �Է� ó��
    if (!PawnOwner->IsLocallyControlled())
        return;

    // 3) UX�� ���� üũ (���������� �ٽ� üũ)
    if (!CanToggleWeapon())
        return;

    // 4) ������ ���� ��� ��û
    Server_ToggleWeapon();
}

void UHBCharacterMafiaAttackComponent::GetLifetimeReplicatedProps(
    TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHBCharacterMafiaAttackComponent, bIsCanActiveEquip);
}

void UHBCharacterMafiaAttackComponent::Server_ToggleWeapon_Implementation()
{
    // 5) �������� ���� ���� (����)
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

    if (bIsCanActiveEquip == false)
        return false;

    return true;
}