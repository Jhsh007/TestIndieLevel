#include "GE_MainCharNormalDamage.h"
#include "TestIndieLevel/AbilitySystem/TIL_AttributeSet.h"

UGE_MainCharNormalDamage::UGE_MainCharNormalDamage(){
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	FGameplayAttribute HealthAttribute = UTIL_AttributeSet::GetHealthAttribute();
	ModifierInfo.Attribute = HealthAttribute;
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(-50.f);
	
	Modifiers.Add(ModifierInfo);
}
