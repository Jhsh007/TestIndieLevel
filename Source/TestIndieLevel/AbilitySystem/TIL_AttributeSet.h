#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TIL_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealthValue, float, MaxHealthValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStaminaValue, float, MaxStaminaValue);

UCLASS()
class TESTINDIELEVEL_API UTIL_AttributeSet : public UAttributeSet{
	GENERATED_BODY()
	
public:
	UTIL_AttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData MaxStamina;

public:
	ATTRIBUTE_ACCESSORS(UTIL_AttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UTIL_AttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UTIL_AttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS(UTIL_AttributeSet, MaxStamina);

	void SetHealth(float NewHealth);
	void SetStamina(float NewStamina);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChanged OnStaminaChanged;

	/*UFUNCTION()
	void HealthChanged(const FGameplayAttribute& Attribute, float NewValue);
	UFUNCTION()
	void StaminaChanged(const FGameplayAttribute& Attribute, float NewValue);*/
};
