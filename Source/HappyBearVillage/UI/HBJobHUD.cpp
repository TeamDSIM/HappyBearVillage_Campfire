// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBJobHUD.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

UHBJobHUD::UHBJobHUD(const FObjectInitializer& ObjectInitializer)
{
}

void UHBJobHUD::UpdateJobInfo(EJobType InJobType)
{
	if (JobImage)
	{
		FString TempJobTextString = FString::Printf(TEXT("Icon_%sBear"),
			*StaticEnum<EJobType>()->GetNameStringByValue(static_cast<int32>(InJobType)));
		JobTextString = StaticEnum<EJobType>()->GetNameStringByValue(static_cast<int32>(InJobType));
		for (UTexture2D* Icon : JobIconArray)
		{
			FString IconName = FString::Printf(TEXT("Icon_%sBear"), *Icon->GetName());
			UE_LOG(LogTemp, Log, TEXT("IconName : %s"), *IconName);
			if (JobTextString == Icon->GetName())
			{
				JobImage->SetBrushFromTexture(Icon);
			}
		}
	}

	if (JobText)
	{
		JobTextString = StaticEnum<EJobType>()->GetNameStringByValue(static_cast<int32>(InJobType));
		JobText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *JobTextString)));
	}
}

void UHBJobHUD::NativeConstruct()
{
	Super::NativeConstruct();

	JobImage = Cast<UImage>(GetWidgetFromName("JobIcon"));
	ensure(JobImage);

	JobText = Cast<UTextBlock>(GetWidgetFromName("JobText"));
	ensure(JobText);
}
