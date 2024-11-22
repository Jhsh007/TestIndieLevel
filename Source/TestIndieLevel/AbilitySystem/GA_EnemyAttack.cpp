#include "GA_EnemyAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TestIndieLevel/BaseEnemy.h"
#include "TestIndieLevel/AI/EnemyAIController.h"

UGA_EnemyAttack::UGA_EnemyAttack(){
	const FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
	AbilityTags.AddTag(AttackTag);
}

void UGA_EnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait =
					UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, EName::None, AttackMontage);
		if(PlayMontageAndWait){
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_EnemyAttack::OnMontageCompleted);
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_EnemyAttack::OnMontageCompleted);
			PlayMontageAndWait->ReadyForActivation();
		}else{
			CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		}
	}else{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
	}
}

void UGA_EnemyAttack::OnMontageCompleted(){
	if(CurrentActorInfo && CurrentActorInfo->OwnerActor.IsValid()){
		ABaseEnemy* EnemyActor= Cast<ABaseEnemy>(CurrentActorInfo->OwnerActor);
		if(EnemyActor){
			AEnemyAIController* AIController = Cast<AEnemyAIController>(EnemyActor->GetController());
			if(AIController){
				AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), true);
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
