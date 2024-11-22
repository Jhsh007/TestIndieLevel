#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_DoubleJump.generated.h"

UCLASS()
class TESTINDIELEVEL_API UGA_DoubleJump : public UGameplayAbility{
	GENERATED_BODY()
	
public:
	UGA_DoubleJump();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);
};
