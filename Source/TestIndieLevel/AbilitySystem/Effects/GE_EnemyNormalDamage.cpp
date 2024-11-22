#include "GE_EnemyNormalDamage.h"
#include "TestIndieLevel/AbilitySystem/TIL_AttributeSet.h"

UGE_EnemyNormalDamage::UGE_EnemyNormalDamage(){
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	FGameplayAttribute HealthAttribute = UTIL_AttributeSet::GetHealthAttribute();
	ModifierInfo.Attribute = HealthAttribute;
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(-30.f);
	
	Modifiers.Add(ModifierInfo);
}
