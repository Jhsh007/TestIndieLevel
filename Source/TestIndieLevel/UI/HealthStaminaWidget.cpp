#include "HealthStaminaWidget.h"
#include "Components/ProgressBar.h"

void UHealthStaminaWidget::UpdateHealth(float NewHealth){
	HealthBar->SetPercent(NewHealth);
}

void UHealthStaminaWidget::UpdateStamina(float NewStamina){
	StaminaBar->SetPercent(NewStamina);
}
