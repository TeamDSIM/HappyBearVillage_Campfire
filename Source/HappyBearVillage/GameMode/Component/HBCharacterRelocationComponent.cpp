// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Component/HBCharacterRelocationComponent.h"

#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "Character/HBCharacterPlayer.h"
#include "Subsystem/VillageGenerationWorldSubsystem.h"

UHBCharacterRelocationComponent::UHBCharacterRelocationComponent() { }

void UHBCharacterRelocationComponent::RelocateCharactersToHouse(AHBMafiaGameState* InGameState)
{
	if (!InGameState) return;
	
	UVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UVillageGenerationWorldSubsystem>();
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

	//  서버에서만 실행
	if (!GetOwner()->HasAuthority())
		return;

    // 설정값
    const FVector Center = FVector(200000.0f, 0.0f, 100.0f); // 중심 위치
    const float Radius = 300.f;                           // 원 반지름

    // 캐릭터 수집
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

    // 원형 배치
  
    const float AngleStep = 360.f / NumPlayers;

    for (int32 i = 0; i < NumPlayers; ++i)
    {
        const float AngleDeg = AngleStep * i;
        const float AngleRad = FMath::DegreesToRadians(AngleDeg);

        FVector TargetLocation;
        TargetLocation.X = Center.X + FMath::Cos(AngleRad) * Radius;
        TargetLocation.Y = Center.Y + FMath::Sin(AngleRad) * Radius;
        TargetLocation.Z = Center.Z;

        //중심을 바라보게 회전
        FRotator LookAtRotation = (Center - TargetLocation).Rotation();
        //BP에서 -90도로 설정되어 있음
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
