#pragma once

#include "CoreMinimal.h"

class HAPPYBEARVILLAGE_API HBUtils
{
public:
	HBUtils() = delete;
	~HBUtils() = delete;

	static void InitRandomSeed(int32 InSeed);
	static float GetRandomFloatFromStream(float Min, float Max);
	static int32 GetRandomInt32FromStream(int32 Min, int32 Max);

	template<typename T>
	static void ShuffleWithStream(TArray<T>& Target)
	{
		for (int i=Target.Num() - 1; i>0; --i)
		{
			int RandomIndex = RandomStream.RandRange(0, i);
			Swap(Target[i], Target[RandomIndex]);
		}
	}

	template<typename T>
	static void Shuffle(TArray<T>& Target)
	{
		for (int i=Target.Num() - 1; i>0; --i)
		{
			int RandomIndex = FMath::RandRange(0, i);
			Swap(Target[i], Target[RandomIndex]);
		}
	}
	

private:
	static FRandomStream RandomStream;
};
