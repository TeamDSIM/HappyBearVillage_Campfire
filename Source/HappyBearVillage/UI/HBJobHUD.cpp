// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBJobHUD.h"

#include "Character/Stat/HBCharacterRole.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UHBJobHUD::UHBJobHUD(const FObjectInitializer& ObjectInitializer)
{
}

void UHBJobHUD::UpdateJobInfo(EJobType InJobType)
{
	if (JobInfoImage)
	{
		FString TempJobTextString = FString::Printf(TEXT("Icon_%sBear"),
			*StaticEnum<EJobType>()->GetNameStringByValue(static_cast<int32>(InJobType)));
		UE_LOG(LogTemp, Warning, TEXT("TempJobTextString : %s"), *TempJobTextString);
		for (UTexture2D* Icon : JobIconArray)
		{
			FString IconName = FString::Printf(TEXT("%s"), *Icon->GetName());
			UE_LOG(LogTemp, Log, TEXT("IconName : %s"), *IconName);
			if (TempJobTextString.Equals(Icon->GetName(), ESearchCase::IgnoreCase))
			{
				JobInfoImage->SetBrushFromTexture(Icon);
			}
		}
	}

	if (JobInfoText)
	{
		JobTextString = StaticEnum<EJobType>()->GetNameStringByValue(static_cast<int32>(InJobType));
		JobInfoText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *JobTextString)));

		int32 JobNum = static_cast<int32>(InJobType);
		
		UE_LOG(LogTemp, Log, TEXT("InitRole Call / JobNum = %d"), JobNum);

		if (JobNum >= static_cast<int32>(EJobType::MAFIA) && JobNum < static_cast<int32>(EJobType::CITIZEN))
		{
			JobInfoText->SetColorAndOpacity(FLinearColor::Red);
		}
		else if(JobNum >= static_cast<int32>(EJobType::CITIZEN) && JobNum < static_cast<int32>(EJobType::END))
		{
			JobInfoText->SetColorAndOpacity(FLinearColor::White);
		}
	}
}

void UHBJobHUD::NativeConstruct()
{
	Super::NativeConstruct();

	JobInfoImage = Cast<UImage>(GetWidgetFromName("JobIcon"));
	ensure(JobInfoImage);

	JobInfoText = Cast<UTextBlock>(GetWidgetFromName("JobText"));
	ensure(JobInfoText);
}
