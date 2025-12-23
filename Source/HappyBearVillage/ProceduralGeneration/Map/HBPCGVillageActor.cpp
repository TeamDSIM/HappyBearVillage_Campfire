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

	
	static ConstructorHelpers::FObjectFinder<UPCGGraph> GraphAsset(TEXT("/Game/Personal/JANG_H_W/Tools/PCG/PCG_TextureTest.PCG_TextureTest"));
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

void AHBPCGVillageActor::Generate()
{
	PCG->Cleanup();
	PCG->Generate();
}

void AHBPCGVillageActor::BeginPlay()
{
	Super::BeginPlay();

	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();
	UHBMapGenerator* MapGenerator = NewObject<UHBMapGenerator>();
	
	FHBNoiseSettings NoiseSettings;
	NoiseSettings.Resolution = {64, 64};
	NoiseSettings.GridSize = {4, 4};
	NoiseSettings.Seed = 0.125;
	
	PerlinNoise->GeneratePerlinNoise(NoiseSettings);

	FHBMapData MapData = MapDataGenerator->GenerateMapData(NoiseSettings);
	
	UTexture2D* MapTexture = MapDataGenerator->GenerateForestTexture2D();

	InitializePCGInput(MapTexture);
	MapGenerator->GenerateVillage(MapData, GetWorld());
}
