#include "MainCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/GA_Jump.h"
#include "AbilitySystem/TIL_AbilitySystemComponent.h"
#include "AbilitySystem/TIL_AttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/GA_DoubleJump.h"
#include "AbilitySystem/Effects/GE_Cost_NormalAbility.h"
#include "Kismet/KismetSystemLibrary.h"

AMainCharacter::AMainCharacter(){
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
    	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTIL_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UTIL_AttributeSet>("AttributeSet");

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

void AMainCharacter::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData){
	UTIL_AttributeSet* TIl_AttributeSet = Cast<UTIL_AttributeSet>(AttributeSet);
	if(TIl_AttributeSet){
		TIl_AttributeSet->OnHealthChanged.Broadcast(OnAttributeChangeData.NewValue, TIl_AttributeSet->GetMaxHealth());
	}
}

void AMainCharacter::OnStaminaChanged(const FOnAttributeChangeData& OnAttributeChangeData){
	UTIL_AttributeSet* TIl_AttributeSet = Cast<UTIL_AttributeSet>(AttributeSet);
	if(TIl_AttributeSet){
		TIl_AttributeSet->OnStaminaChanged.Broadcast(OnAttributeChangeData.NewValue, TIl_AttributeSet->GetMaxStamina());
	}
}

void AMainCharacter::InitializeAbilitySystem(){
	if(AbilitySystemComponent){
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		UTIL_AbilitySystemComponent* TIL_AbilitySystem = Cast<UTIL_AbilitySystemComponent>(AbilitySystemComponent);
		if(TIL_AbilitySystem){
			TIL_AbilitySystem->AddAbilities(StartupAbilities);
		}

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UTIL_AttributeSet::GetHealthAttribute())
							  .AddUObject(this, &AMainCharacter::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UTIL_AttributeSet::GetStaminaAttribute())
		                      .AddUObject(this, &AMainCharacter::OnStaminaChanged);
	}
}

void AMainCharacter::BeginPlay(){
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnOverlapEnd);

	InitializeAbilitySystem();
}

void AMainCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if(bInterpCamera){
		InterpElapsedTime += DeltaTime;
		const float Alpha = FMath::Clamp(InterpElapsedTime/InterpDuration, 0.0f, 1.0f);
		if(Alpha >= 1){
			bInterpCamera = false;
			InterpElapsedTime = 0;
		}
		
		if(bIsOnCrouch){
			const float CurrentHeight = FMath::Lerp(StandHeight, CrouchHeight, Alpha);
			FVector NewRelativeLocation = GetMesh()->GetRelativeLocation();
			const float CurrentRelativeLoc = FMath::Lerp(MeshStandRelativeLoc, MeshCrouchRelativeLoc, Alpha);
			NewRelativeLocation.Z = CurrentRelativeLoc;
			GetCapsuleComponent()->SetCapsuleHalfHeight(CurrentHeight);
			GetMesh()->SetRelativeLocation(NewRelativeLocation);
		}else{
			const float CurrentHeight = FMath::Lerp(CrouchHeight, StandHeight, Alpha);
			FVector NewRelativeLocation = GetMesh()->GetRelativeLocation();
			const float CurrentRelativeLoc = FMath::Lerp(MeshCrouchRelativeLoc, MeshStandRelativeLoc, Alpha);
			NewRelativeLocation.Z = CurrentRelativeLoc;
			GetCapsuleComponent()->SetCapsuleHalfHeight(CurrentHeight);
			GetMesh()->SetRelativeLocation(NewRelativeLocation);
		}
	}
}

void AMainCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
}

void AMainCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(InputAction_Action1, ETriggerEvent::Triggered, this, &AMainCharacter::Action1);
	Input->BindAction(InputAction_Action2, ETriggerEvent::Triggered, this, &AMainCharacter::Action2);
	Input->BindAction(InputAction_Action3, ETriggerEvent::Triggered, this, &AMainCharacter::Action3);
	Input->BindAction(InputAction_Action4, ETriggerEvent::Triggered, this, &AMainCharacter::Action4);
	Input->BindAction(InputAction_Action5, ETriggerEvent::Triggered, this, &AMainCharacter::Action5);
}

void AMainCharacter::PossessedBy(AController* NewController){
	Super::PossessedBy(NewController);
	
	if(AbilitySystemComponent){
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

UAbilitySystemComponent* AMainCharacter::GetAbilitySystemComponent() const{
	return AbilitySystemComponent;
}

void AMainCharacter::MoveForward(float Value){
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value){
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::Turn(float Value){
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value){
	AddControllerPitchInput(Value);
}

void AMainCharacter::Action1(){
	if(!AbilitySystemComponent) return;
	
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag(FName("Input.Action1"));
	ActivateAbilityForInput(InputTag);
}

void AMainCharacter::Action2(){
	if(!AbilitySystemComponent) return;
	
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag(FName("Input.Action2"));
	ActivateAbilityForInput(InputTag);
}

void AMainCharacter::Action3(){
	if(!AbilitySystemComponent) return;

	/*TODO:HardAttack*/
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag(FName("Input.Action3"));
	ActivateAbilityForInput(InputTag);
}

void AMainCharacter::Action4(){
	if(!AbilitySystemComponent) return;
	
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag(FName("Input.Action4"));
	ActivateAbilityForInput(InputTag);
}

void AMainCharacter::Action5(){
	if(!AbilitySystemComponent) return;
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag(FName("Input.Action5"));
	ActivateAbilityForInput(InputTag);
}

void AMainCharacter::Landed(const FHitResult& Hit){
	Super::Landed(Hit);

	if(AbilitySystemComponent){
		UE_LOG(LogTemp, Warning, TEXT("Firing Event.Land"));
		const FGameplayTag EvenTag = FGameplayTag::RequestGameplayTag(FName("Event.Land"));
		FGameplayEventData EventData;
		EventData.EventTag = EvenTag;
		EventData.Instigator = this;
		AbilitySystemComponent->HandleGameplayEvent(EvenTag, &EventData);
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Fail ASC"));
	}
}

void AMainCharacter::BeginCrouch(){
	bInterpCamera = true;
	bIsOnCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}

void AMainCharacter::EndCrouch(){
	bInterpCamera = true;
	bIsOnCrouch = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMainCharacter::ApplyAttack(){
	const FTransform HandTransform = GetMesh()->GetBoneTransform(TEXT("hand_r"));
	const FVector HandLocation = HandTransform.GetLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	FHitResult SphereHitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, HandLocation, HandLocation, 20.f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, SphereHitResult, true);

	if(SphereHitResult.bBlockingHit){
		AActor* HitActor = SphereHitResult.GetActor();
		if(HitActor){
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if(ASC && DamageEffectClass){
				const FGameplayTag HitTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Hit"));
				
				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1, ASC->MakeEffectContext());
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				ASC->ExecuteGameplayCue(HitTag);
			}
		}
	}
}

void AMainCharacter::BeginCounter(){
	bIsDefending = false;
	if(AbilitySystemComponent && CounterMontage){
		PlayAnimMontage(CounterMontage, 1, TEXT("Counter"));
		
		const FGameplayTag EvenTag = FGameplayTag::RequestGameplayTag(FName("Event.SuccessCounter"));
		FGameplayEventData EventData;
		EventData.EventTag = EvenTag;
		EventData.Instigator = this;
		AbilitySystemComponent->HandleGameplayEvent(EvenTag, &EventData);
	}
}

void AMainCharacter::ActivateAbilityForInput(FGameplayTag InputTag){
	if (!AbilitySystemComponent) return;
	
	if(AbilityInputs.Contains(InputTag)){
		// Get the ability class associated with this input
		TSubclassOf<UGameplayAbility> AbilityClass = *AbilityInputs.Find(InputTag);
		
		if(AbilityClass){
			// Check if the ability class is derived from UGA_Jump
			if(AbilityClass->IsChildOf(UGA_Jump::StaticClass())){
				if(GetCharacterMovement()->IsFalling()){
					AbilitySystemComponent->TryActivateAbilityByClass(UGA_DoubleJump::StaticClass());
					return;
				}
			}
			AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass);
		}
	}
}
