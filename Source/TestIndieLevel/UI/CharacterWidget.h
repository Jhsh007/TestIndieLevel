#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

class UHealthStaminaWidget;

UCLASS()
class TESTINDIELEVEL_API UCharacterWidget : public UUserWidget{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UHealthStaminaWidget* HealthStaminaWidget;

public:
	UFUNCTION()
	FORCEINLINE UHealthStaminaWidget* GetHealthStaminaWidget() const { return HealthStaminaWidget; };
};
