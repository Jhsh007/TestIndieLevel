#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharAnimInstance.generated.h"

class UCharacterMovementComponent;
class AMainCharacter;

UCLASS()
class TESTINDIELEVEL_API UMainCharAnimInstance : public UAnimInstance{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Basic")
	TObjectPtr<AMainCharacter> MainCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Basic")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Movement")
	bool ShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Movement")
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Movement")
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Movement")
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Movement")
	bool bIsOnCrouch;
};