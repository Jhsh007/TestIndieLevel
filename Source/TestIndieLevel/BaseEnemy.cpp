#include "BaseEnemy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MainCharacter.h"
#include "AbilitySystem/TIL_AbilitySystemComponent.h"
#include "AbilitySystem/TIL_AttributeSet.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/EnemyHealthBar.h"

ABaseEnemy::ABaseEnemy(){
	PrimaryActorTick.bCanEverTick = true;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("Health Widget");
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidget->SetDrawAtDesiredSize(true);
	HealthWidget->SetupAttachment(GetRootComponent());
	
	AbilitySystemComponent = CreateDefaultSubobject<UTIL_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UTIL_AttributeSet>("AttributeSet");
}

void ABaseEnemy::BeginPlay(){
	Super::BeginPlay();

	InitializeAbilitySystem();

	if(EnemyAIController)
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), true);
	
	UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(HealthWidget->GetWidget());
	if(HealthBar){
		UTIL_AttributeSet* TIL_AttributeSet = Cast<UTIL_AttributeSet>(AttributeSet);
		if(TIL_AttributeSet){
			UE_LOG(LogTemp, Warning, TEXT("Binded"));
			HealthBar->BindWidget(TIL_AttributeSet);
		}
	}
}

void ABaseEnemy::InitializeAbilitySystem(){
	if(AbilitySystemComponent){
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		UTIL_AbilitySystemComponent* TIL_AbilitySystem = Cast<UTIL_AbilitySystemComponent>(AbilitySystemComponent);
		if(TIL_AbilitySystem){
			TIL_AbilitySystem->AddAbilities(StartupAbilities);
		}
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UTIL_AttributeSet::GetHealthAttribute())
							  .AddUObject(this, &ABaseEnemy::OnHealthChanged);
	}
}

void ABaseEnemy::PossessedBy(AController* NewController){
	Super::PossessedBy(NewController);

	EnemyAIController = Cast<AEnemyAIController>(NewController);
	if(BehaviorTree){
		EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		EnemyAIController->RunBehaviorTree(BehaviorTree);
	}
}

void ABaseEnemy::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* ABaseEnemy::GetAbilitySystemComponent() const{
	return AbilitySystemComponent;
}

void ABaseEnemy::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData){
	UTIL_AttributeSet* TIl_AttributeSet = Cast<UTIL_AttributeSet>(AttributeSet);
	if(TIl_AttributeSet){
		TIl_AttributeSet->OnHealthChanged.Broadcast(OnAttributeChangeData.NewValue, TIl_AttributeSet->GetMaxHealth());
	}
}

void ABaseEnemy::ApplyAttack(){
	const FTransform HandTransform = GetMesh()->GetBoneTransform(TEXT("hand_l"));
	const FVector HandLocation = HandTransform.GetLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	FHitResult SphereHitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, HandLocation, HandLocation, 20.f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, SphereHitResult, true);

	if(SphereHitResult.bBlockingHit){
		AMainCharacter* HitActor = Cast<AMainCharacter>(SphereHitResult.GetActor());
		if(HitActor){
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if(ASC && DamageEffectClass){
				if(EnemyAIController && EnemyAIController->GetBlackboardComponent()){
					if(HitActor->GetIsDefending()){
						HitActor->BeginCounter();
						EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
						EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("WasHitted"), true);
					}else{
						FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1, ASC->MakeEffectContext());
						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		}
	}
}

