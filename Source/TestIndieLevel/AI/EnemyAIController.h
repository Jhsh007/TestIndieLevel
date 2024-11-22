#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "EnemyAIController.generated.h"

struct FAIStimulus;
class UBehaviorTreeComponent;

UCLASS()
class TESTINDIELEVEL_API AEnemyAIController : public AAIController{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	void SetTargetInBlackboard(AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sight")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
};
