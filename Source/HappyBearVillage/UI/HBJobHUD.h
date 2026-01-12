// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBJobHUD.generated.h"

enum class EJobType : uint8;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBJobHUD(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateJobInfo(EJobType InJobType);
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UTexture2D>> JobIconArray;
	
	UPROPERTY()
	TObjectPtr<UImage> JobImage;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> JobText;
	
	UPROPERTY()
	FString JobTextString;
};
