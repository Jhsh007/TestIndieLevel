#include "GE_MainCharHardDamage.h"
#include "TestIndieLevel/AbilitySystem/TIL_AttributeSet.h"

UGE_MainCharHardDamage::UGE_MainCharHardDamage(){
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	FGameplayAttribute HealthAttribute = UTIL_AttributeSet::GetHealthAttribute();
	ModifierInfo.Attribute = HealthAttribute;
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(-100.f);
	
	Modifiers.Add(ModifierInfo);
}
