// Fill out your copyright notice in the Description page of Project Settings.


#include "HBBlockingVolume.h"
#include "Components/BoxComponent.h"

AHBBlockingVolume::AHBBlockingVolume()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	
	BoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BoxComponent->SetBoxExtent(FVector(50.f, 50.f, 500.f));
}