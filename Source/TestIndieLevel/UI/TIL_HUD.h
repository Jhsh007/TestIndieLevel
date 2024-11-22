#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TIL_HUD.generated.h"

struct FOnAttributeChangeData;
class UCharacterWidget;
class UHealthStaminaWidget;

UCLASS()
class TESTINDIELEVEL_API ATIL_HUD : public AHUD{
	GENERATED_BODY()

public:

	ATIL_HUD();

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateHealthBar(float Value, float MaxValue);
	UFUNCTION()
	void UpdateStaminaBar(float Value, float MaxValue);

protected:
	UPROPERTY()
	UCharacterWidget* MainWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainWidgetClass;
};
