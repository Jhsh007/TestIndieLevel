#include "TIL_HUD.h"

#include "CharacterWidget.h"
#include "HealthStaminaWidget.h"
#include "Blueprint/UserWidget.h"
#include "TestIndieLevel/MainCharacter.h"
#include "TestIndieLevel/AbilitySystem/TIL_AttributeSet.h"

ATIL_HUD::ATIL_HUD(){
}

void ATIL_HUD::BeginPlay(){
	Super::BeginPlay();
	
	AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(GetOwningPawn());
	APlayerController* PC = PlayerCharacter->GetLocalViewingPlayerController();
	if(PlayerCharacter && PC){
		UTIL_AttributeSet* AttributeSet = Cast<UTIL_AttributeSet>(PlayerCharacter->GetAttributeSet());
		if(AttributeSet){
			AttributeSet->OnHealthChanged.AddDynamic(this, &ATIL_HUD::UpdateHealthBar);
			AttributeSet->OnStaminaChanged.AddDynamic(this, &ATIL_HUD::UpdateStaminaBar);

			MainWidget = CreateWidget<UCharacterWidget>(PC, MainWidgetClass);
			if(MainWidget){
				MainWidget->AddToViewport();
				MainWidget->GetHealthStaminaWidget()->UpdateHealth(AttributeSet->GetHealth());
				MainWidget->GetHealthStaminaWidget()->UpdateStamina(AttributeSet->GetStamina());
			}
			//PlayerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, HealthChanged())
		}
	}
}

void ATIL_HUD::UpdateHealthBar(float Value, float MaxValue){
	if(MainWidget){
		const float Percent = Value/MaxValue;
		MainWidget->GetHealthStaminaWidget()->UpdateHealth(Percent);
	}
}

void ATIL_HUD::UpdateStaminaBar(float Value, float MaxValue){
	if(MainWidget){
		const float Percent = Value/MaxValue;
		MainWidget->GetHealthStaminaWidget()->UpdateStamina(Percent);
	}
}