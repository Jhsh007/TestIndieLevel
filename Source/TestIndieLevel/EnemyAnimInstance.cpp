#include "EnemyAnimInstance.h"
#include "BaseEnemy.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation(){
	Super::NativeInitializeAnimation();

	EnemyCharacter = Cast<ABaseEnemy>(TryGetPawnOwner());
	if(EnemyCharacter)
		MovementComponent = EnemyCharacter->GetCharacterMovement();
	
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds){
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(EnemyCharacter){
		Velocity = MovementComponent->Velocity;
		const FVector2d VelocityXY = FVector2d(Velocity.X, Velocity.Y);
		GroundSpeed = VelocityXY.Length();
		if(GroundSpeed > 3.f){
			ShouldMove = true;
		}else{
			ShouldMove = false;
		}
		bIsFalling = MovementComponent->IsFalling();
	}
}