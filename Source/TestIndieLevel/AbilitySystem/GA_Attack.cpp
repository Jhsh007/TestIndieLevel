#include "GA_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Effects/GE_Cost_NormalAbility.h"
#include "TestIndieLevel/MainCharacter.h"

UGA_Attack::UGA_Attack(){
	/* Tags */
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
	const FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.DoubleJump"));
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	const FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(FName("Ability.Dash"));

	AbilityTags.AddTag(AttackTag);

	CancelAbilitiesWithTag.AddTag(CrouchTag);
	
	BlockAbilitiesWithTag.AddTag(JumpTag);
	BlockAbilitiesWithTag.AddTag(DoubleJumpTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(DashTag);
	BlockAbilitiesWithTag.AddTag(AttackTag);

	//Cost effect
	CostGameplayEffectClass = UGE_Cost_NormalAbility::StaticClass();
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character && AttackMontage){
			if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
				UAbilityTask_PlayMontageAndWait* PlayMontageAndWait =
					UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, EName::None, AttackMontage);
				if(PlayMontageAndWait){
					PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Attack::OnMontageCompleted);
					PlayMontageAndWait->ReadyForActivation();
				}else{
					CancelAbility(Handle, ActorInfo, ActivationInfo, false);
				}
			}else{
				CancelAbility(Handle, ActorInfo, ActivationInfo, false);
			}
		}else{
			CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		}
	}else{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
	}
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled){
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Attack::OnMontageCompleted(){
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}