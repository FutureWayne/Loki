// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/LokiCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "LokiEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class LOKI_API ALokiEnemy : public ALokiCharacterBase
{
	GENERATED_BODY()

public:
	ALokiEnemy();

	virtual void BeginPlay() override;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	/** Combat Interface */
	virtual int32 GetCharacterLevel() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealtChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealtChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

protected:
	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void Die() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
