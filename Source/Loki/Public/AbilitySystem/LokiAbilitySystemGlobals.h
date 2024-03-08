// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "LokiAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class LOKI_API ULokiAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
