#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TIL_AbilitySystemComponent.generated.h"

UCLASS()
class TESTINDIELEVEL_API UTIL_AbilitySystemComponent : public UAbilitySystemComponent{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
};
