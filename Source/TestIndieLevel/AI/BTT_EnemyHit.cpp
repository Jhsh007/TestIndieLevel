#include "BTT_EnemyHit.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TestIndieLevel/BaseEnemy.h"

EBTNodeResult::Type UBTT_EnemyHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(EnemyAIController){
		ABaseEnemy* SelfPawn = Cast<ABaseEnemy>(EnemyAIController->GetPawn());
		if(SelfPawn){
			USkeletalMeshComponent* Mesh = SelfPawn->GetMesh();
			if(Mesh){
				Mesh->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UBTT_EnemyHit::OnMontageCompleted);
				SelfPawn->PlayAnimMontage(HitMontage, 1, TEXT("Hit"));
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

void UBTT_EnemyHit::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted){
	if(BehaviorTreeComp){
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(BehaviorTreeComp->GetAIOwner());
		if(EnemyAIController){
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("WasHitted"), false);
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), true);
		}
		// After the montage is finished, mark the task as complete
		FinishLatentTask(*BehaviorTreeComp, EBTNodeResult::Succeeded);
	}
}
