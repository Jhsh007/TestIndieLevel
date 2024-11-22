#include "GE_Cost_NormalAbility.h"

#include "TestIndieLevel/AbilitySystem/TIL_AttributeSet.h"

UGE_Cost_NormalAbility::UGE_Cost_NormalAbility(){
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	FGameplayAttribute StaminaAttribute = UTIL_AttributeSet::GetStaminaAttribute();
	ModifierInfo.Attribute = StaminaAttribute;
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(-10.f);
	
	Modifiers.Add(ModifierInfo);
}
