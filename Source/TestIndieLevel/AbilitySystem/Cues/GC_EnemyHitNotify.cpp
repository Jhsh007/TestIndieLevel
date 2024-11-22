#include "GC_EnemyHitNotify.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "TestIndieLevel/AI/EnemyAIController.h"

AGC_EnemyHitNotify::AGC_EnemyHitNotify(){
	PrimaryActorTick.bCanEverTick = true;

	const FGameplayTag CueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Hit"));
	bAutoDestroyOnRemove = true;

	GameplayCueTag = CueTag;
}

bool AGC_EnemyHitNotify::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters){
	Super::OnExecute_Implementation(MyTarget, Parameters);
	
	ACharacter* Character = Cast<ACharacter>(MyTarget);
	if(Character){
		AEnemyAIController* AIController = Cast<AEnemyAIController>(Character->GetController());
		if(AIController){
			ApplyHit(Character, AIController);
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
		}
		return true;
	}
	
	return false; 
}

void AGC_EnemyHitNotify::BeginPlay(){
	Super::BeginPlay();
	
}

void AGC_EnemyHitNotify::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AGC_EnemyHitNotify::ApplyHit(ACharacter* Char, AEnemyAIController* Controller){
	if(HitMontage)
		Char->PlayAnimMontage(HitMontage);
	Controller->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), true);

}

