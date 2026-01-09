// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPCGVillageActor.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "Elements/PCGTextureSampler.h"
#include "Components/BoxComponent.h"

#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/Map/HBMapGenerator.h"

AHBPCGVillageActor::AHBPCGVillageActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	PCG = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));
	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("PCGBounds"));
	Bounds->SetupAttachment(Mesh);
	Bounds->SetCollisionProfileName(TEXT("NoCollision"));
	
	static ConstructorHelpers::FObjectFinder<UPCGGraph> GraphAsset(TEXT("/Game/Personal/JANG_H_W/Tools/PCG/PCG_Village.PCG_Village"));
	if (GraphAsset.Succeeded())
	{
		PCG->SetGraph(GraphAsset.Object);
	}
}

void AHBPCGVillageActor::InitializePCGInput(UTexture2D* Texture2D)
{
	UPCGGraph* Graph = PCG->GetGraph();

	for (UPCGNode* Node : Graph->GetNodes())
	{
		UPCGSettings* Settings = Node->GetSettings();
		if (!Settings) continue;

		if (Settings->IsA<UPCGTextureSamplerSettings>())
		{
			UPCGTextureSamplerSettings* TextureSettings = Cast<UPCGTextureSamplerSettings>(Settings);
			UE_LOG(LogTemp, Log, TEXT("%s Found!"), *Settings->GetName());

			TextureSettings->SetTexture(Texture2D);
		}
	}
}

void AHBPCGVillageActor::SetPCGBounds()
{
	SetActorLocation(FVector(12600.f, 12600.f, 0.f));
	Bounds->SetBoxExtent(FVector(12800.f, 12800.f, 1000.f));
}

void AHBPCGVillageActor::Generate()
{
	PCG->Generate();
}

void AHBPCGVillageActor::BeginPlay()
{
	Super::BeginPlay();
}
