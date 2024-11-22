#include "GA_Dash.h"

#include "TIL_AttributeSet.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Effects/GE_Cost_NormalAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TestIndieLevel/MainCharacter.h"

UGA_Dash::UGA_Dash(){
	const FGameplayTag DashTag = FGameplayTag::RequestGameplayTag(FName("Ability.Dash"));
	AbilityTags.AddTag(DashTag);
	/* Cost */
	CostGameplayEffectClass = UGE_Cost_NormalAbility::StaticClass();
	/* Cancel abilities */
	const FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
	const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.DoubleJump"));
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	const FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("Ability.Attack"));
	CancelAbilitiesWithTag.AddTag(JumpTag);
	CancelAbilitiesWithTag.AddTag(DoubleJumpTag);
	CancelAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(JumpTag);
	BlockAbilitiesWithTag.AddTag(DoubleJumpTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
	BlockAbilitiesWithTag.AddTag(AttackTag);
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
				/* Check if there's a velocity to define the direction of movement
				 * If not use the forward vector
				 */
				const bool bIsAirDash = Character->GetCharacterMovement()->IsFalling();
				FVector Velocity = Character->GetVelocity();
				Velocity.Z = 0;
				FVector Direction = Character->GetActorForwardVector();
				if(Velocity.Length()>0){
					Direction = Velocity.GetSafeNormal();
				}
				//If is jumping or not the dash force change, in the ground needs more force to compensate friction
				Velocity = bIsAirDash ? Direction*DashForceAir : Direction*DashForce;
				Character->LaunchCharacter(Velocity, true, true);
				
				Character->PlayAnimMontage(DashMontage);
				/* Wait 0.5f sec to end the ability */
				UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, WaitDelayTime);
				if(WaitTask){
					WaitTask->OnFinish.AddDynamic(this, &UGA_Dash::OnDelayCompleted);
					WaitTask->ReadyForActivation();
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

void UGA_Dash::OnDelayCompleted(){
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
