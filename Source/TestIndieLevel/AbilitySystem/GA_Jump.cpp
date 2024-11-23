#include "GA_Jump.h"
#include "AbilitySystemComponent.h"
#include "TestIndieLevel/MainCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_Jump::UGA_Jump(){
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	
	AbilityTags.AddTag(JumpTag);
	
	CancelAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
				/* Start the jump and wait until land event is trigger */
				FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Land"));
				Character->Jump();
				UAbilityTask_WaitGameplayEvent* WaitForEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
				if(WaitForEventTask){
					WaitForEventTask->EventReceived.AddDynamic(this, &UGA_Jump::OnEventReceived);
					WaitForEventTask->ReadyForActivation();
				}else{
					CancelAbility(Handle, ActorInfo, ActivationInfo, false);
				}
			}
		}else{
			CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		}
	}else{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
	}
}

void UGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled){
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Jump::OnEventReceived(FGameplayEventData Payload){
	if(Payload.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Land"))){
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}