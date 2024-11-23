#include "GA_Uncrouch.h"

UGA_Uncrouch::UGA_Uncrouch(){
	const FGameplayTag UncrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Uncrouch"));
	const FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.DoubleJump"));
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	const FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(FName("Ability.Dash"));

	AbilityTags.AddTag(UncrouchTag);
	
	CancelAbilitiesWithTag.AddTag(CrouchTag);
	
	BlockAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(UncrouchTag);
	BlockAbilitiesWithTag.AddTag(AttackTag);
	BlockAbilitiesWithTag.AddTag(JumpTag);
	BlockAbilitiesWithTag.AddTag(DoubleJumpTag);
	BlockAbilitiesWithTag.AddTag(DashTag);
}

void UGA_Uncrouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
		/* uncrouch just cancel the crouch ability */
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}
