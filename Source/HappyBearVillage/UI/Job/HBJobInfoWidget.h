// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBJobInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RefreshJobIcon();
	void RefreshJobName();
	void RefreshJobDescription();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Job")
	TObjectPtr<class UImage> JobIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> JobText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> JobDescText;
};
