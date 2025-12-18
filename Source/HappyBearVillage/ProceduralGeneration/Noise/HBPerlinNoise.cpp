// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/Noise/HBPerlinNoise.h"

void UHBPerlinNoise::GeneratePerlinNoise(FHBNoiseSettings InNoiseSettings)
{
	NoiseSettings = InNoiseSettings;
	
	if (InNoiseSettings.Resolution.X <= 0 || InNoiseSettings.Resolution.Y <= 0)
	{
		return;
	}

	for (int32 Row = 0; Row < 200; Row++)
	{
		for (int32 Col = 0; Col < 200; Col++)
		{
			Noise[Row][Col] = 0;
		}
	}
	
	for (int32 Row = 0; Row < NoiseSettings.Resolution.Y; Row++)
	{
		for (int32 Col = 0; Col < NoiseSettings.Resolution.X; Col++)
		{
			FVector2D UV;
			UV.X = static_cast<float>(Col) / NoiseSettings.Resolution.X;
			UV.Y = static_cast<float>(Row) / NoiseSettings.Resolution.Y;
			UV = FVector2D(UV.X * NoiseSettings.GridSize.X, UV.Y * NoiseSettings.GridSize.Y);
			
			FIntVector2 GridID;
			GridID.X = FMath::FloorToInt(UV.X);
			GridID.Y = FMath::FloorToInt(UV.Y);

			FVector2D GridUV;
			GridUV.X = FMath::Frac(UV.X);
			GridUV.Y = FMath::Frac(UV.Y);

			FIntVector2 TopLeft = GridID + FIntVector2(0, 0);
			FIntVector2 TopRight = GridID + FIntVector2(1, 0);
			FIntVector2 BottomLeft = GridID + FIntVector2(0, 1);
			FIntVector2 BottomRight = GridID + FIntVector2(1, 1);

			FVector2D GradientTopLeft = GetRandomGradient(TopLeft, NoiseSettings.Seed);
			FVector2D GradientTopRight = GetRandomGradient(TopRight, NoiseSettings.Seed);
			FVector2D GradientBottomLeft = GetRandomGradient(BottomLeft, NoiseSettings.Seed);
			FVector2D GradientBottomRight = GetRandomGradient(BottomRight, NoiseSettings.Seed);

			FVector2D OffsetFromTopLeft = GridUV - FVector2D(0, 0);
			FVector2D OffsetFromTopRight = GridUV - FVector2D(1, 0);
			FVector2D OffsetFromBottomLeft = GridUV - FVector2D(0, 1);
			FVector2D OffsetFromBottomRight = GridUV - FVector2D(1, 1);

			float DotTopLeft = FVector2D::DotProduct(GradientTopLeft, OffsetFromTopLeft);
			float DotTopRight = FVector2D::DotProduct(GradientTopRight, OffsetFromTopRight);
			float DotBottomLeft = FVector2D::DotProduct(GradientBottomLeft, OffsetFromBottomLeft);
			float DotBottomRight = FVector2D::DotProduct(GradientBottomRight, OffsetFromBottomRight);

			FVector2D SmoothUV;
			SmoothUV.X = FMath::SmoothStep(0.0f, 1.0f, static_cast<float>(GridUV.X));
			SmoothUV.Y = FMath::SmoothStep(0.0f, 1.0f, static_cast<float>(GridUV.Y));
			
			float InterpolateTop = FMath::Lerp(DotTopLeft, DotTopRight, SmoothUV.X);
			float InterpolateBottom = FMath::Lerp(DotBottomLeft, DotBottomRight, SmoothUV.X);
			float Perlin = FMath::Lerp(InterpolateTop, InterpolateBottom, SmoothUV.Y);

			Perlin = FMath::Clamp(Perlin * 20, -9.9f, 9.9f);
			Noise[Row][Col] = static_cast<int32>(Perlin);
		}
	}
}

void UHBPerlinNoise::DebugGeneratePerlinNoise(FHBNoiseSettings InNoiseSettings)
{
	NoiseSettings = InNoiseSettings;
	
	if (InNoiseSettings.Resolution.X <= 0 || InNoiseSettings.Resolution.Y <= 0)
	{
		return;
	}

	FString UVLogTest;
	FString GridIDLogText;
	FString GridUVLogText;
	
	FString BottomRightLogText;
	FString GradientBottomRightLogText;
	FString OffsetFromBottomRightLogText;
	FString DotBottomRightLogText;
	
	FString SmoothUVLogText;
	FString PerlinLogText;

	for (int32 Row = 0; Row < NoiseSettings.Resolution.Y; Row++)
	{
		for (int32 Col = 0; Col < NoiseSettings.Resolution.X; Col++)
		{
			FVector2D UV;
			UV.X = static_cast<float>(Col) / NoiseSettings.Resolution.X;
			UV.Y = static_cast<float>(Row) / NoiseSettings.Resolution.Y;
			UV = FVector2D(UV.X * NoiseSettings.GridSize.X, UV.Y * NoiseSettings.GridSize.Y);

			UVLogTest += FString::Printf(TEXT("(%0.1f, %0.1f) "), UV.X, UV.Y);
			
			FIntVector2 GridID;
			GridID.X = FMath::FloorToInt(UV.X);
			GridID.Y = FMath::FloorToInt(UV.Y);
			
			GridIDLogText += FString::Printf(TEXT("(%d, %d) "), GridID.X, GridID.Y);

			FVector2D GridUV;
			GridUV.X = FMath::Frac(UV.X);
			GridUV.Y = FMath::Frac(UV.Y);

			GridUVLogText += FString::Printf(TEXT("(%d, %d) "), GridID.X, GridID.Y);

			FIntVector2 TopLeft = GridID + FIntVector2(0, 0);
			FIntVector2 TopRight = GridID + FIntVector2(1, 0);
			FIntVector2 BottomLeft = GridID + FIntVector2(0, 1);
			FIntVector2 BottomRight = GridID + FIntVector2(1, 1);

			BottomRightLogText += FString::Printf(TEXT("(%d, %d) "), BottomRight.X, BottomRight.Y);

			FVector2D GradientTopLeft = GetRandomGradient(TopLeft, NoiseSettings.Seed);
			FVector2D GradientTopRight = GetRandomGradient(TopRight, NoiseSettings.Seed);
			FVector2D GradientBottomLeft = GetRandomGradient(BottomLeft, NoiseSettings.Seed);
			FVector2D GradientBottomRight = GetRandomGradient(BottomRight, NoiseSettings.Seed);

			GradientBottomRightLogText += FString::Printf(TEXT("(%0.1f, %0.1f) "), GradientBottomRight.X, GradientBottomRight.Y);

			FVector2D OffsetFromTopLeft = GridUV - FVector2D(0, 0);
			FVector2D OffsetFromTopRight = GridUV - FVector2D(1, 0);
			FVector2D OffsetFromBottomLeft = GridUV - FVector2D(0, 1);
			FVector2D OffsetFromBottomRight = GridUV - FVector2D(1, 1);

			OffsetFromBottomRightLogText += FString::Printf(TEXT("(%0.1f, %0.1f) "), OffsetFromBottomRight.X, OffsetFromBottomRight.Y);

			float DotTopLeft = FVector2D::DotProduct(GradientTopLeft, OffsetFromTopLeft);
			float DotTopRight = FVector2D::DotProduct(GradientTopRight, OffsetFromTopRight);
			float DotBottomLeft = FVector2D::DotProduct(GradientBottomLeft, OffsetFromBottomLeft);
			float DotBottomRight = FVector2D::DotProduct(GradientBottomRight, OffsetFromBottomRight);

			DotBottomRightLogText += FString::Printf(TEXT("(%0.1f) "), DotBottomRight);

			FVector2D SmoothUV;
			SmoothUV.X = FMath::SmoothStep(0.0f, 1.0f, static_cast<float>(GridUV.X));
			SmoothUV.Y = FMath::SmoothStep(0.0f, 1.0f, static_cast<float>(GridUV.Y));

			SmoothUVLogText += FString::Printf(TEXT("(%0.1f, %0.1f) "), SmoothUV.X, SmoothUV.Y);
			
			float InterpolateTop = FMath::Lerp(DotTopLeft, DotTopRight, SmoothUV.X);
			float InterpolateBottom = FMath::Lerp(DotBottomLeft, DotBottomRight, SmoothUV.X);
			float Perlin = FMath::Lerp(InterpolateTop, InterpolateBottom, SmoothUV.Y);

			Perlin = FMath::Clamp(Perlin * 20, -9.9f, 9.9f);
			PerlinLogText += FString::Printf(TEXT("(%0.1f)"), Perlin);
			
			Noise[Row][Col] = static_cast<int32>(Perlin);
		}

		UVLogTest += TEXT("\n");
		GridIDLogText += TEXT("\n");
		GridUVLogText += TEXT("\n");
		
		BottomRightLogText += TEXT("\n");
		GradientBottomRightLogText += TEXT("\n");
		OffsetFromBottomRightLogText += TEXT("\n");
		DotBottomRightLogText += TEXT("\n");
		
		SmoothUVLogText += TEXT("\n");
		PerlinLogText += TEXT("\n");
	}

	UE_LOG(LogTemp, Log, TEXT("[Coord (Row, Col)] : \n%s"), *UVLogTest);
	UE_LOG(LogTemp, Log, TEXT("[GridID (Row, Col)] : \n%s"), *GridIDLogText);
	UE_LOG(LogTemp, Log, TEXT("[BottomRight (Row, Col)] : \n%s"), *BottomRightLogText);
	UE_LOG(LogTemp, Log, TEXT("[GradientBottomRight (Row, Col)] : \n%s"), *GradientBottomRightLogText);
	UE_LOG(LogTemp, Log, TEXT("[OffsetFromBottomRight (Row, Col)] : \n%s"), *OffsetFromBottomRightLogText);
	UE_LOG(LogTemp, Log, TEXT("[DotBottomRight] : \n%s"), *DotBottomRightLogText);
	UE_LOG(LogTemp, Log, TEXT("[SmoothCoord (Row, Col)] : \n%s"), *SmoothUVLogText);
	UE_LOG(LogTemp, Log, TEXT("[Perlin] : \n%s"), *PerlinLogText);
}

void UHBPerlinNoise::PrintNoise()
{
	if (NoiseSettings.Resolution.X <= 0 || NoiseSettings.Resolution.Y <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PrintNoise() Resolution Error"));
		return;
	}

	FString LogText;

	for (int32 Row=0; Row<NoiseSettings.Resolution.X; ++Row)
	{
		for (int32 Col=0; Col<NoiseSettings.Resolution.Y; ++Col)
		{
			if (Noise[Row][Col] >= 0)
			{
				LogText += FString::Printf(TEXT("%d"), Noise[Row][Col]);
			}
			else
			{
				LogText += TEXT(" ");
			}
		}

		LogText += TEXT("\n");
	}

	UE_LOG(LogTemp, Log, TEXT("[Noise Array] : \n%s"), *LogText);
}

TArray<int32> UHBPerlinNoise::GetNoiseAs1D()
{
	TArray<int32> Result;

	for (int32 Row = 0; Row < NoiseSettings.Resolution.Y; Row++)
	{
		for (int32 Col = 0; Col < NoiseSettings.Resolution.X; Col++)
		{
			Result.Add(Noise[Row][Col]);
		}
	}

	return Result;
}

int32 UHBPerlinNoise::GetNoiseElement(int32 Row, int32 Col)
{
	if (NoiseSettings.Resolution.X <= 0 || NoiseSettings.Resolution.Y <= 0)
	{
		return -100;
	}

	if (Row < 0 || Col < 0 || Row >= NoiseSettings.Resolution.Y || Col >= NoiseSettings.Resolution.X)
	{
		return -100;
	}

	return Noise[Row][Col];
}

FVector2D UHBPerlinNoise::GetRandomGradient(FIntVector2 Position, float Seed)
{
	FVector2D AdjustedPosition = FVector2D(Position.X + 0.02f, Position.Y + 0.02f);

	float X = FVector2D::DotProduct(AdjustedPosition, FVector2D(123.4f, 234.5f));
	float Y = FVector2D::DotProduct(AdjustedPosition, FVector2D(234.5f, 345.6f));

	FVector2D Gradient = FVector2D(FMath::Sin(X), FMath::Sin(Y)) * 43758.5453f;

	Gradient.X = FMath::Sin(Gradient.X + Seed);
	Gradient.Y = FMath::Sin(Gradient.Y + Seed);
	Gradient.Normalize();

	return Gradient;
}