#include "GA_Jump.h"
#include "AbilitySystemComponent.h"
#include "TestIndieLevel/MainCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_Jump::UGA_Jump(){
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	AbilityTags.AddTag(JumpTag);

	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	CancelAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
				FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Land"));
				Character->Jump();
				UAbilityTask_WaitGameplayEvent* WaitForEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
				if(WaitForEventTask){
					UE_LOG(LogTemp, Warning, TEXT("Correct WaitForEventTask"));
					WaitForEventTask->EventReceived.AddDynamic(this, &UGA_Jump::OnEventReceived);
					WaitForEventTask->ReadyForActivation();
					UE_LOG(LogTemp, Warning, TEXT("WaitForEventTask Event Received Delegate Added"));
				}else{
					UE_LOG(LogTemp, Warning, TEXT("Fail WaitForEventTask"));
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
	UE_LOG(LogTemp, Warning, TEXT("Enter to OnEventReceived"));
	if(Payload.EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Land"))){
		UE_LOG(LogTemp, Warning, TEXT("Success Payload"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Fail Payload"));
	}
}

/*bool UGA_Jump::CheckJump(AMainCharacter* Target, UAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo){
	/* Check if the character is on any state
	bool bHasStateTag = false;
	const FGameplayTag StatePrefixTag = FGameplayTag::RequestGameplayTag(FName("State"));
	FGameplayTagContainer ActorTags;
	ASC->GetOwnedGameplayTags(ActorTags);

	for(const FGameplayTag& Tag : ActorTags){
		if(Tag.ToString().StartsWith(StatePrefixTag.GetTagName().ToString())){
			bHasStateTag = true;
			break;
		}
	}
	/* If there's a state and that state is jumping can do a double jump 
	if(bHasStateTag){
		const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("State.Jumping"));
		if(ASC->HasMatchingGameplayTag(JumpTag)){
			const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("State.Jumping.Double"));
			if(!ASC->HasMatchingGameplayTag(DoubleJumpTag)){
				const UTIL_AttributeSet* AS = Cast<UTIL_AttributeSet>(Target->GetAttributeSet());
				if(AS){
					UGameplayEffect* Cost = UGE_Cost_NormalAbility::StaticClass()->GetDefaultObject<UGameplayEffect>();;
					if(ASC->CanApplyAttributeModifiers(Cost, GetAbilityLevel(Handle, ActorInfo), MakeEffectContext(Handle, ActorInfo))){
						ASC->AddLooseGameplayTag(DoubleJumpTag);
						return true;
					}
				}
			}
		}
	}else{//if there's no state, can do a normal jump
		const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("State.Jumping"));
		ASC->AddLooseGameplayTag(JumpTag);
		return true;
	}
	return false;
}*/

/*
if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
			/* Checks if a jump or double jump can be done 
			if(CheckJump(Character, ASC, Handle, ActorInfo)){
				const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("State.Jumping.Double"));
				if(ASC->HasMatchingGameplayTag(JumpTag)){//If is a double jump
					if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
						FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
						EffectContextHandle.AddSourceObject(Character);
						const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(UGE_Cost_NormalAbility::StaticClass(), 1.f, EffectContextHandle);
						ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());//Creates and apply a cost effect
						JumpAction(Character, true);
						EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
					}else{
						CancelAbility(Handle, ActorInfo, ActivationInfo, false);
					}
				}else{
					/* If is a normal jump there's no cost 
					JumpAction(Character, false);
					EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
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
*/