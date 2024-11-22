#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TestIndieLevel/MainCharacter.h"

AEnemyAIController::AEnemyAIController(){
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	SightConfig->SightRadius = 3000.f;
	SightConfig->LoseSightRadius = 3500.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Add the sight sense to the perception component
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::BeginPlay(){
	Super::BeginPlay();

	if(BlackboardComponent){
		BlackboardComponent->SetValueAsBool(TEXT("CanAttack"), true);
	}
}

void AEnemyAIController::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus){
	if(Actor && Actor->IsA(AMainCharacter::StaticClass())){
		SetTargetInBlackboard(Actor);
		return;
	}
	SetTargetInBlackboard(nullptr);
}

void AEnemyAIController::SetTargetInBlackboard(AActor* Actor){
	if (BlackboardComponent){
		BlackboardComponent->SetValueAsObject("Target", Actor);
	}
}

