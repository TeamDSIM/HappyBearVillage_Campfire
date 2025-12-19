// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/HBPlayerRoleWidget.h"

#include "Components/TextBlock.h"
#include "Interface/HBCharacterWidgetInterface.h"

UHBPlayerRoleWidget::UHBPlayerRoleWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UHBPlayerRoleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TextBlock 불러오기
	RoleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Role")));
	ensure(RoleText);

	JobText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Job")));
	ensure(JobText);

	// 인터페이스로 캐릭터에 위젯 붙이기
	IHBCharacterWidgetInterface* CharacterWidget = Cast<IHBCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetUpCharacterWidget(this);
	}
}

void UHBPlayerRoleWidget::UpdatePlayerRole(ERoleType NewRole)
{
	PlayerRole = StaticEnum<ERoleType>()->GetNameStringByValue(static_cast<int32>(NewRole));

	if (RoleText)
	{
		RoleText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *PlayerRole)));
	}
	
}

void UHBPlayerRoleWidget::UpdatePlayerJob(EJobType NewJob)
{
	PlayerJob = StaticEnum<EJobType>()->GetNameStringByValue(static_cast<int32>(NewJob));

	if (JobText)
	{
		JobText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *PlayerJob)));
	}
}
