#include "BTT_EnemyAttack.h"
#include "AbilitySystemComponent.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TestIndieLevel/BaseEnemy.h"

EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(EnemyAIController){
		ABaseEnemy* SelfPawn = Cast<ABaseEnemy>(EnemyAIController->GetPawn());
		if(SelfPawn){
			UAbilitySystemComponent* EnemyASC = SelfPawn->GetAbilitySystemComponent();
			if(EnemyASC && AttackAbilityClass){
				/* If the data is correct activate attack ability
				 * and stop the AI process(follow target to attack it) because it's now in the process of attack
				 */
				if(EnemyASC->TryActivateAbilityByClass(AttackAbilityClass)){
					EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
					return EBTNodeResult::Succeeded;
				}
				return EBTNodeResult::Failed;
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
