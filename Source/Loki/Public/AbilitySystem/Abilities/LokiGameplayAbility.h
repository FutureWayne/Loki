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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
