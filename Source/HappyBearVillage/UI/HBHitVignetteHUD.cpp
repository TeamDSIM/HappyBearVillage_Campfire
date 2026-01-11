// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBHitVignetteHUD.h"
#include "Components/Image.h"


void UHBHitVignetteHUD::NativeConstruct()
{
    Super::NativeConstruct();

    //기본 상태는 숨기기
    SetVisibility(ESlateVisibility::Collapsed);
}

void UHBHitVignetteHUD::PlayHit(float Intensity)
{
    if (!VignetteImage || !HitPulse) return;

    //범위를 0~1로 설정
    const float A = FMath::Clamp(Intensity, 0.f, 1.f);

    //이미지의 알파 값을 강도에 따라 조절
    FLinearColor C = VignetteImage->ColorAndOpacity;
    C.A = A;
    VignetteImage->SetColorAndOpacity(C);

    SetVisibility(ESlateVisibility::Visible);
    StopAllAnimations();
    PlayAnimation(HitPulse, 0.f, 1);
}