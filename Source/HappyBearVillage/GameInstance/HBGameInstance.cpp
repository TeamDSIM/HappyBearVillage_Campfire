// Fill out your copyright notice in the Description page of Project Settings.


#include "HBGameInstance.h"
#include "Controller/HBPlayerController.h"
#include "Kismet/GameplayStatics.h"

UHBGameInstance::UHBGameInstance()
{

}

void UHBGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UHBGameInstance::OnPostLoadMap);
}


void UHBGameInstance::OnPostLoadMap(UWorld* World)
{
    UE_LOG(LogTemp, Log, TEXT("OnPostLoadMap Loaded"));

    if (!World)
    {
        return;
    }
    bNewMapLoad = true;

    //FirstPlayerController 를 가져오면, client 의 PC가 아닌 host의 PC를 가져옴
    //근데 0번이 자기꺼맞는데?
    //"Local"의 PC를 가져와야 함..?
    //APlayerController* PC = World->GetFirstPlayerController();
    //APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    //AHBPlayerController* HBPC = Cast<AHBPlayerController>(PC);
    //if (!HBPC || !HBPC->IsLocalController())
    //{
    //    UE_LOG(LogTemp, Log, TEXT("no localcontroller >> OnPostLoadMap"));
    //    return;
    //}

    //결론: OnPostLoadMap 함수는 맵 로드 끝난 직후이지만, 네트워크/플레이어/컨트롤러/possession은 안끝났을수도 있다.
    //AHBPlayerController* LocalHBPC = nullptr;

    //for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    //{
    //    UE_LOG(LogTemp, Log, TEXT("for문진입"));
    //    APlayerController* PC = It->Get();
    //    UE_LOG(LogTemp, Warning, TEXT("PC=%s Local=%d"), *GetNameSafe(PC), PC ? PC->IsLocalController() : -1);

    //    if (PC && PC->IsLocalController())
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("Local PC class = %s"), *GetNameSafe(PC->GetClass()));
    //        LocalHBPC = Cast<AHBPlayerController>(PC);
    //        UE_LOG(LogTemp, Warning, TEXT("Cast to AHBPlayerController = %s"), LocalHBPC ? TEXT("SUCCESS") : TEXT("FAIL"));
    //        break;
    //    }
    //}

    //if (!LocalHBPC)
    //{
    //    UE_LOG(LogTemp, Log, TEXT("OnPostLoadMap: LocalHBPC not found"));
    //    return;
    //}



    //LocalHBPC->ResetUI();
    //LocalHBPC->SetupUI();

}