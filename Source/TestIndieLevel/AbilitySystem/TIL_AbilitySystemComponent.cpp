#include "TIL_AbilitySystemComponent.h"

void UTIL_AbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities){
	for(TSubclassOf<UGameplayAbility> AbilityClass : Abilities){
		UE_LOG(LogTemp, Warning, TEXT("abilities"));
		const FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityClass);
		GiveAbility(Spec);
	}
}

