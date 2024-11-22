#include "GA_DoubleJump.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Effects/GE_Cost_NormalAbility.h"
#include "TestIndieLevel/MainCharacter.h"

UGA_DoubleJump::UGA_DoubleJump(){
	const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.DoubleJump"));
	AbilityTags.AddTag(DoubleJumpTag);
	CostGameplayEffectClass = UGE_Cost_NormalAbility::StaticClass();
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	CancelAbilitiesWithTag.AddTag(JumpTag);
	BlockAbilitiesWithTag.AddTag(JumpTag);
	BlockAbilitiesWithTag.AddTag(DoubleJumpTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
}

void UGA_DoubleJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
				FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Land"));
				Character->LaunchCharacter(FVector(0, 0, 1) * Character->GetSecondJumpForce(), false, true);
				UAbilityTask_WaitGameplayEvent* WaitForEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
				if(WaitForEventTask){
					WaitForEventTask->EventReceived.AddDynamic(this, &UGA_DoubleJump::OnEventReceived);
					WaitForEventTask->ReadyForActivation();
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

void UGA_DoubleJump::OnEventReceived(FGameplayEventData Payload){
	if(Payload.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Land"))){
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}