// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Component/HBCharacterRelocationComponent.h"

#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "InteractObject/HBExecutePlatform.h"
#include "Subsystem/HBGameVoteSubsystem.h"
#include "Subsystem/HBVillageGenerationWorldSubsystem.h"

UHBCharacterRelocationComponent::UHBCharacterRelocationComponent() { }

void UHBCharacterRelocationComponent::RelocateCharactersToHouse(AHBMafiaGameState* InGameState)
{
	if (!InGameState) return;
	
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	if (!VillageGenerationSystem) return;

	const TMap<FLinearColor, FVector>& HouseLocationsByColor = VillageGenerationSystem->GetMapData().HouseLocationsByColor;
	
	for (APlayerState* Player : InGameState->PlayerArray)
	{
		AController* PlayerContoller = Player->GetPlayerController();
		if (!PlayerContoller)
		{
			UE_LOG(LogYS, Log, TEXT("Failed Character Relocate To House - 1"));
			return;
		}

		AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerContoller->GetPawn());
		if (!Character)
		{
			UE_LOG(LogYS, Log, TEXT("Failed Character Relocate To House - 2"));
			return;
		}

		// ���� ����� �̵� ����
		if (Character->GetStat()->GetIsAlive() == false)
		{
			continue;
		}
		
		FLinearColor CharacterColor = Character->PlayerColor;
		FVector HouseLocation = HouseLocationsByColor[CharacterColor];
		HouseLocation = FVector(HouseLocation.X, HouseLocation.Y, 200); 
		Character->SetActorLocation(HouseLocation);
	}
}

void UHBCharacterRelocationComponent::RelocateCharacterToCouncil(AHBMafiaGameState* InGameState)
{
	if (!InGameState) return;
	if (!GetOwner() || !GetWorld()) return;

	//  ���������� ����
	if (!GetOwner()->HasAuthority())
		return;

    // ������
    const FVector Center = FVector(200000.0f, 0.0f, 100.0f); // �߽� ��ġ
    const float Radius = 300.f;                           // �� ������

    // ĳ���� ����
    TArray<AHBCharacterPlayer*> Characters;

    for (APlayerState* PS : InGameState->PlayerArray)
    {
        if (!PS) continue;

        AController* PC = PS->GetPlayerController();
        if (!PC) continue;

        AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PC->GetPawn());
        if (!Character) continue;

        Characters.Add(Character);
    }

    const int32 NumPlayers = Characters.Num();
    if (NumPlayers == 0) return;

    // ���� ��ġ
  
    const float AngleStep = 360.f / NumPlayers;

    for (int32 i = 0; i < NumPlayers; ++i)
    {
    	// ���� ����� �̵� ����
		if (Characters[i]->GetStat()->GetIsAlive() == false)
		{
			continue;
		}
    	
        const float AngleDeg = AngleStep * i;
        const float AngleRad = FMath::DegreesToRadians(AngleDeg);

        FVector TargetLocation;
        TargetLocation.X = Center.X + FMath::Cos(AngleRad) * Radius;
        TargetLocation.Y = Center.Y + FMath::Sin(AngleRad) * Radius;
        TargetLocation.Z = Center.Z;

        //�߽��� �ٶ󺸰� ȸ��
        FRotator LookAtRotation = (Center - TargetLocation).Rotation();
        //BP���� -90���� �����Ǿ� ����
        //LookAtRotation.Yaw += 90.f;
        AController* Con = Characters[i]->GetController();
        if (Con)
        {
            Con->SetControlRotation(LookAtRotation);
        }

        Characters[i]->TeleportTo(TargetLocation, LookAtRotation, false, true);
        Characters[i]->SetActorRotation(LookAtRotation);
	}
}

void UHBCharacterRelocationComponent::RelocateCharacterToExecutePlatform(AHBMafiaGameState* InGameState)
{
	UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
	if (VoteSubsystem)
	{
		AHBCharacterPlayer* Target = VoteSubsystem->GetCurrentVoteTarget();
		if (Target)
		{
			// ó���� ��ġ�� �̵�
			Target->SetActorLocation(VoteSubsystem->GetExecutePlatform()->GetTargetPosition());
		}
	}
}
