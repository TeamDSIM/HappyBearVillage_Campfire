#include "UI/HBNightStaminaWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

void UHBNightStaminaWidget::SetStamina(int32 NewStamina)
{
	int32 Clamped = FMath::Clamp(NewStamina, 0, 3);

	if (Img_Stamina_1)
	{
		Img_Stamina_1->SetVisibility(Clamped >= 1 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (Img_Stamina_2)
	{
		Img_Stamina_2->SetVisibility(Clamped >= 2 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (Img_Stamina_3)
	{
		Img_Stamina_3->SetVisibility(Clamped >= 3 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}