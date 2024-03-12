// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LokiGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LOKI_API ULokiGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void ConfirmCurrentAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void CancelCurrentAbility();

	UFUNCTION()
	void OnMouseClick(FGameplayTag InputTag);

};
