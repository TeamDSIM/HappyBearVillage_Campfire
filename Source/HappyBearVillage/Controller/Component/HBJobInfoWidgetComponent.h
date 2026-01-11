// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBJobInfoWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBJobInfoWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHBJobInfoWidgetComponent();

	void ShowJobInfoWidget(APlayerController* InPlayerController);

protected:
	void HideJobInfoWidget();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<class UHBJobInfoWidget> JobInfoWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBJobInfoWidget> JobInfoWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	FTimerHandle HideTimerHandler;
};
