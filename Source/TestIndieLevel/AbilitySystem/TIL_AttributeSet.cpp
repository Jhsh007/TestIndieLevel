#include "TIL_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "TIL_AbilitySystemComponent.h"

UTIL_AttributeSet::UTIL_AttributeSet(){
	InitHealth(500.f);
	InitMaxHealth(500.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
}

void UTIL_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue){
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute()){
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if(Attribute == GetStaminaAttribute()){
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UTIL_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue){
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(Attribute == GetHealthAttribute()){
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if(Attribute == GetStaminaAttribute()){
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UTIL_AttributeSet::SetHealth(float NewHealth){
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	if(ensure(AbilityComp)){
		NewHealth = FMath::Clamp(NewHealth, 0, GetMaxHealth());
		AbilityComp->SetNumericAttributeBase(GetHealthAttribute(), NewHealth);
		OnHealthChanged.Broadcast(NewHealth, GetMaxHealth());
	};
}

void UTIL_AttributeSet::SetStamina(float NewStamina){
	UE_LOG(LogTemp, Warning, TEXT("Ability NewStamina: %f"), NewStamina);
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	if(ensure(AbilityComp)){
		NewStamina = FMath::Clamp(NewStamina, 0, GetMaxStamina());
		AbilityComp->SetNumericAttributeBase(GetStaminaAttribute(), NewStamina);
		UE_LOG(LogTemp, Warning, TEXT("Ability NewStamina Broadcasted: %f"), NewStamina);
		OnStaminaChanged.Broadcast(NewStamina, GetMaxStamina());
	};
}
