#pragma once

#include "CoreMinimal.h"

class HAPPYBEARVILLAGE_API HBUtils
{
public:
	HBUtils() = delete;
	~HBUtils() = delete;

	template<typename T>
	static void Shuffle(TArray<T>& Target)
	{
		for (int i=Target.Num() - 1; i>0; --i)
		{
			int RandomIndex = FMath::RandHelper(i);		// ToDo : 동기화 과정에 대체 필요
			Swap(Target[i], Target[RandomIndex]);
		}
	}
};
