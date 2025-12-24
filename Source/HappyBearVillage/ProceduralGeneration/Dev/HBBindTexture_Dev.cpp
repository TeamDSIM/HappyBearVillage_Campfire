// Fill out your copyright notice in the Description page of Project Settings.


#include "HBBindTexture_Dev.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "Components/BoxComponent.h"
#include "Data/PCGTextureData.h"
#include "Elements/PCGTextureSampler.h"


AHBBindTexture_Dev::AHBBindTexture_Dev()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	PCG = CreateDefaultSubobject<UPCGComponent>(TEXT("PCGComponent"));
	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("PCGBounds"));
}

void AHBBindTexture_Dev::BeginPlay()
{
	Super::BeginPlay();
	
	UPCGGraph* Graph = PCG->GetGraph();
	UPCGNode* Input = Graph->GetInputNode();
	
	UTexture2D* RuntimeTexture = UTexture2D::CreateTransient(64, 64);
	UPCGTextureData* TextureData = NewObject<UPCGTextureData>();

	for (UPCGNode* Node : Graph->GetNodes())
	{
		UPCGSettings* Settings = Node->GetSettings();
		if (!Settings) continue;

		if (Settings->IsA<UPCGTextureSamplerSettings>())
		{
			UPCGTextureSamplerSettings* TextureSettings = Cast<UPCGTextureSamplerSettings>(Settings);
			
			FString Name = Settings->GetName();
			UE_LOG(LogTemp, Log, TEXT("%s Found!"), *Name);

			TextureSettings->SetTexture(RuntimeTexture);

			// Input->SetSettingsInterface(TextureSettings);
		}
	}
	
	PCG->Generate();
}
