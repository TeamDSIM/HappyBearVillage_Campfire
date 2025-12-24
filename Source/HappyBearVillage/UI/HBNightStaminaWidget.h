#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBNightStaminaWidget.generated.h"

class UImage;

/**
 * C++로 Stamina 이미지를 제어하는 서브 위젯
 * WBP_NightStamina 블루프린트의 Parent Class 로 설정하세요 (클래스: HBNightStaminaWidget)
 * 그리고 내부 이미지 이름은 Img_Stamina_1 / Img_Stamina_2 / Img_Stamina_3 로 맞춥니다.
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBNightStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Set stamina (0..MaxStamina)
	UFUNCTION(BlueprintCallable, Category = "Night")
	void SetStamina(int32 NewStamina);

protected:
	// BindWidget 으로 UMG에서 만든 Image 위젯과 연결됩니다.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Stamina_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Stamina_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_Stamina_3;
};