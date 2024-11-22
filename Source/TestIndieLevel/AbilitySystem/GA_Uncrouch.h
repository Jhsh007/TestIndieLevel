#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Uncrouch.generated.h"

UCLASS()
class TESTINDIELEVEL_API UGA_Uncrouch : public UGameplayAbility{
	GENERATED_BODY()

public:
	UGA_Uncrouch();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
