#pragma once

#include "CoreMinimal.h"
#include "TIL_GameplayAbility.h"
#include "GA_Dash.generated.h"

UCLASS()
class TESTINDIELEVEL_API UGA_Dash : public UTIL_GameplayAbility{
	GENERATED_BODY()

	UGA_Dash();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnDelayCompleted();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashForce = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashForceAir = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WaitDelayTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DashMontage;
};
