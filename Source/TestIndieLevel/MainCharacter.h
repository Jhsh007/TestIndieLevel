#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MainCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UGameplayEffect;
class UAttributeSet;
class UGameplayAbility;
class UAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ECharacterStates : uint8 {
	TIL_NONE UMETA(DisplayName = "NONE"),
	TIL_JUMPING UMETA(DisplayName = "JUMPING"),
	TIL_CROUCHED UMETA(DisplayName = "CROUCHED"),
	TIL_ATTACKING UMETA(DisplayName = "ATTACKING")
};

UCLASS()
class TESTINDIELEVEL_API AMainCharacter : public ACharacter, public IAbilitySystemInterface{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	void OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnStaminaChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void InitializeAbilitySystem();
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; };
	
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable)
	void Turn(float Value);
	UFUNCTION(BlueprintCallable)
	void LookUp(float Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputAction_Action1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction_Action2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction_Action3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction_Action4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction_Action5;
	
	UFUNCTION(BlueprintCallable)
	void Action1();
	UFUNCTION(BlueprintCallable)
	void Action2();
	UFUNCTION(BlueprintCallable)
	void Action3();
	UFUNCTION(BlueprintCallable)
	void Action4();
	UFUNCTION(BlueprintCallable)
	void Action5();
	
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void BeginCrouch();
	UFUNCTION(BlueprintCallable)
	void EndCrouch();

	UFUNCTION(BlueprintCallable)
	void ApplyAttack();
	UFUNCTION()
	void BeginCounter();

	UFUNCTION()
	void ActivateAbilityForInput(FGameplayTag InputTag);

	FORCEINLINE float GetSecondJumpForce() const { return SecondJumpForce; };
	FORCEINLINE bool GetIsOnCrouch() const { return  bIsOnCrouch; };
	FORCEINLINE bool GetIsDefending() const { return  bIsDefending; };
	FORCEINLINE void SetIsDefending(bool IsDefending){ bIsDefending = IsDefending; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
	bool bCanDoubleJump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float SecondJumpForce = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crouch")
	bool bIsOnCrouch = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crouch")
	bool bInterpCamera = false;
	
	float InterpElapsedTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch")
	float InterpDuration = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch")
	float StandHeight = 88.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch")
	float CrouchHeight = 65.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch")
	float MeshStandRelativeLoc = -90.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouch")
	float MeshCrouchRelativeLoc = -67.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bIsDefending = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Attack")
	TObjectPtr<UAnimMontage> CounterMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> AbilityInputs;
	
};