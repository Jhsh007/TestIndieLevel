#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Counter.generated.h"

UCLASS()
class TESTINDIELEVEL_API UGA_Counter : public UGameplayAbility{
	GENERATED_BODY()
	
public:
	UGA_Counter();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCanceled();
	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> CounterMontage;
};
