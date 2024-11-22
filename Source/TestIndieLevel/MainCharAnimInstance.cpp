#include "MainCharAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainCharAnimInstance::NativeInitializeAnimation(){
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if(MainCharacter)
		MovementComponent = MainCharacter->GetCharacterMovement();
	
}

void UMainCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds){
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(MainCharacter){
		Velocity = MovementComponent->Velocity;
		const FVector2d VelocityXY = FVector2d(Velocity.X, Velocity.Y);
		GroundSpeed = VelocityXY.Length();
		if(GroundSpeed > 3.f){
			ShouldMove = true;
		}else{
			ShouldMove = false;
		}
		bIsFalling = MovementComponent->IsFalling();
		bIsOnCrouch = MainCharacter->GetIsOnCrouch();
	}
}