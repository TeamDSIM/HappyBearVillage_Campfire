// Fill out your copyright notice in the Description page of Project Settings.


#include "HBJobInfoWidgetComponent.h"
#include "UI/Job/HBJobInfoWidget.h"


UHBJobInfoWidgetComponent::UHBJobInfoWidgetComponent()
{
	static ConstructorHelpers::FClassFinder<UHBJobInfoWidget> JobInfoWidgetClassRef(TEXT("/Game/UI/Jobs/WBP_JobInfoWidget.WBP_JobInfoWidget_C"));
	if (JobInfoWidgetClassRef.Class)
	{
		JobInfoWidgetClass = JobInfoWidgetClassRef.Class;
	}
}

void UHBJobInfoWidgetComponent::ShowJobInfoWidget(APlayerController* InPlayerController)
{
	JobInfoWidget = CreateWidget<UHBJobInfoWidget>(InPlayerController, JobInfoWidgetClass);
	JobInfoWidget->AddToViewport();

	GetWorld()->GetTimerManager().SetTimer(HideTimerHandler, this, &UHBJobInfoWidgetComponent::HideJobInfoWidget, 3.0f);
}

void UHBJobInfoWidgetComponent::HideJobInfoWidget()
{
	JobInfoWidget->RemoveFromParent();
	JobInfoWidget = nullptr;
}
