#include "HBUtils.h"

FRandomStream HBUtils::RandomStream;

void HBUtils::InitRandomSeed(int32 InSeed)
{
	RandomStream.Initialize(InSeed);
}

float HBUtils::GetRandomFloatFromStream(float Min, float Max)
{
	return RandomStream.FRandRange(Min, Max);
}

int32 HBUtils::GetRandomInt32FromStream(int32 Min, int32 Max)
{
	return RandomStream.RandRange(Min, Max);
}