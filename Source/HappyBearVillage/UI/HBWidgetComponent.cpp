// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBWidgetComponent.h"

#include "HBUserWidget.h"

void UHBWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UHBUserWidget* HBUserWidget = Cast<UHBUserWidget>(GetWidget());
	if (HBUserWidget)
	{
		HBUserWidget->SetOwningActor(GetOwner());
	}
}
