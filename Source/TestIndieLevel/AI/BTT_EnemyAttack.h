#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_EnemyAttack.generated.h"

class UGameplayAbility;

UCLASS()
class TESTINDIELEVEL_API UBTT_EnemyAttack : public UBTTask_BlueprintBase{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> AttackAbilityClass;
};
