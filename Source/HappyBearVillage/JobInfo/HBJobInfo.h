#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HBJobInfo.generated.h"

USTRUCT(BlueprintType)
struct FHBJobInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="JobInfo")
	//FString JobName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JobInfo")
	FString JobNameDisplay = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JobInfo")
	FString JobInfo1 = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JobInfo")
	FString JobInfo2 = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JobInfo")
	FString JobInfoDetail1 = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JobInfo")
	FString JobInfoDetail2 = TEXT("");
};