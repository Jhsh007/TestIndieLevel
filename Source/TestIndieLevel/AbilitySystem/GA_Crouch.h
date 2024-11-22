#pragma once

#include "CoreMinimal.h"
#include "TIL_GameplayAbility.h"
#include "GA_Crouch.generated.h"

UCLASS()
class TESTINDIELEVEL_API UGA_Crouch : public UTIL_GameplayAbility{
	GENERATED_BODY()

public:
	UGA_Crouch();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
