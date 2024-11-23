#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GC_EnemyHitNotify.generated.h"

class AEnemyAIController;

UCLASS()
class TESTINDIELEVEL_API AGC_EnemyHitNotify : public AGameplayCueNotify_Actor{
	GENERATED_BODY()

public:
	AGC_EnemyHitNotify();

	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	/* Play the hit montage */
	void ApplyHit(ACharacter* Char, AEnemyAIController* Controller);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitMontage;
};
