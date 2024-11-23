#include "GA_Crouch.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "TestIndieLevel/MainCharacter.h"

UGA_Crouch::UGA_Crouch(){
	const FGameplayTag CrouchTag = FGameplayTag::RequestGameplayTag(FName("Ability.Crouch"));
	AbilityTags.AddTag(CrouchTag);
	
	const FGameplayTag DoubleJumpTag = FGameplayTag::RequestGameplayTag(FName("Ability.DoubleJump"));
	BlockAbilitiesWithTag.AddTag(DoubleJumpTag);
	BlockAbilitiesWithTag.AddTag(CrouchTag);
}

void UGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData){
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			if(!Character->GetCharacterMovement()->IsFalling()){
				if(CommitAbility(Handle, ActorInfo, ActivationInfo)){
					/* If the data is correct begin the crouch process(camera and mesh handling) */
					Character->BeginCrouch();
				}
			}
		}
	}
}

void UGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled){
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if(ActorInfo->OwnerActor.IsValid()){
		AMainCharacter* Character = Cast<AMainCharacter>(ActorInfo->OwnerActor);
		if(Character){
			/* If the data is correct end the crouch process(camera and mesh handling) */
			Character->EndCrouch();
		}
	}
}
