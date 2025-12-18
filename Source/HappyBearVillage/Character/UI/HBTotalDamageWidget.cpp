// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/HBTotalDamageWidget.h"

#include "Components/TextBlock.h"
#include "Interface/HBCharacterWidgetInterface.h"

UHBTotalDamageWidget::UHBTotalDamageWidget(const FObjectInitializer& ObjectInitializer)
{
	CurrentTotalDamage = 0.f;
}

void UHBTotalDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이름으로 TextBlock 불러오기
	TotalDamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalDamage")));
	ensure(TotalDamageText);

	// 인터페이스로 캐릭터에 위젯 붙이기
	IHBCharacterWidgetInterface* CharacterWidget = Cast<IHBCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}

void UHBTotalDamageWidget::UpdateTotalDamage(float NewTotalDamage)
{
	CurrentTotalDamage = NewTotalDamage;
	
	if (TotalDamageText)
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateTotalDamage Call"));
		TotalDamageText->SetText(FText::FromString(FString::Printf(
			TEXT("%.0f"), CurrentTotalDamage)));
	}
}
