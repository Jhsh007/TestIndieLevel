#include "EnemyHealthBar.h"
#include "Components/ProgressBar.h"
#include "TestIndieLevel/AbilitySystem/TIL_AttributeSet.h"

void UEnemyHealthBar::NativePreConstruct(){
	Super::NativePreConstruct();
	
}

void UEnemyHealthBar::UpdateHealth(float Value, float MaxValue){
	const float Percent = Value/MaxValue;
	HealthBar->SetPercent(Percent);
}

void UEnemyHealthBar::BindWidget(UTIL_AttributeSet* AttributeSet){
	AttributeSet->OnHealthChanged.AddDynamic(this, &UEnemyHealthBar::UpdateHealth);
}
