#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

class UTIL_AttributeSet;
class UProgressBar;

UCLASS()
class TESTINDIELEVEL_API UEnemyHealthBar : public UUserWidget{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	UFUNCTION()
	void UpdateHealth(float Value, float MaxValue);

	void BindWidget(UTIL_AttributeSet* AttributeSet);

protected:
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
