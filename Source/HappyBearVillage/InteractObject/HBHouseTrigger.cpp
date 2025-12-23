#include "HBHouseTrigger.h"

#include "Components/BoxComponent.h"
#include "Character/HBCharacterPlayer.h"

AHBHouseTrigger::AHBHouseTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    // Collision ¼³Á¤
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    TriggerBox->SetGenerateOverlapEvents(true);

    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this, &AHBHouseTrigger::OnBeginOverlap
    );
    TriggerBox->OnComponentEndOverlap.AddDynamic(
        this, &AHBHouseTrigger::OnEndOverlap
    );
}

void AHBHouseTrigger::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(OtherActor))
    {
        Player->EnterHouse();
    }
}

void AHBHouseTrigger::OnEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
)
{
    if (AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(OtherActor))
    {
        Player->ExitHouse();
    }
}
