#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthStaminaWidget.generated.h"

class UProgressBar;

UCLASS()
class TESTINDIELEVEL_API UHealthStaminaWidget : public UUserWidget{
	GENERATED_BODY()

public:
	void UpdateHealth(float NewHealth);
	void UpdateStamina(float NewStamina);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	
};
