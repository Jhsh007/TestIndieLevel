#include "GA_Counter.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Effects/GE_Cost_NormalAbility.h"
#include "TestIndieLevel/MainCharacter.h"

UGA_Counter::UGA_Counter(){
	/* Tags */
	const FGameplayTag CounterTag = FGameplayTag::RequestGameplayTag(FName("Ability.Counter"));
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	const FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
	const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.DoubleJump"));
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	const FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(FName("Ability.Dash"));

	AbilityTags.AddTag(CounterTag);

	CancelAbilitiesWithTag.AddTag(AttackTag);
	CancelAbilitiesWithTag.AddTag(CrouchTag);
	
	BlockAbilitiesWithTag.AddTag(JumpTag);
	BlockAbilitiesWithTag.AddTag(DoubleJumpTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(DashTag);
	BlockAbilitiesWithTag.AddTag(AttackTag);

	//Cost effect
	CostGameplayEffectClass = UGE_Cost_NormalAbility::StaticClass();
}

void UGA_Counter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character && CounterMontage){
			if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
				UAbilityTask_PlayMontageAndWait* PlayMontageAndWait =
					UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, EName::None, CounterMontage, 1, TEXT("Section4"));
				if(PlayMontageAndWait){
					Character->SetIsDefending(true);
					PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Counter::OnMontageCompleted);
					PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_Counter::OnMontageCanceled);
					PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_Counter::OnMontageInterrupted);
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

void UGA_Counter::OnMontageCompleted(){
	if(CurrentActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(CurrentActorInfo->OwnerActor);
		if(Character){
			Character->SetIsDefending(false);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Counter::OnMontageInterrupted(){
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Event.SuccessCounter"));
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
	if(WaitGameplayEvent){
		WaitGameplayEvent->EventReceived.AddDynamic(this, &UGA_Counter::OnEventReceived);
		WaitGameplayEvent->ReadyForActivation();
	}
}

void UGA_Counter::OnMontageCanceled(){
	if(CurrentActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(CurrentActorInfo->OwnerActor);
		if(Character){
			Character->SetIsDefending(false);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Counter::OnEventReceived(FGameplayEventData Payload){
	if(Payload.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.SuccessCounter"))){
		if(CurrentActorInfo->OwnerActor.IsValid()){
			AMainCharacter* Character = Cast<AMainCharacter>(CurrentActorInfo->OwnerActor);
			if(Character){
				Character->SetIsDefending(false);
			}
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}
