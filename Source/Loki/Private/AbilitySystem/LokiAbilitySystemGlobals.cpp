// Copyright Ludens Studio


#include "AbilitySystem/LokiAbilitySystemGlobals.h"
#include "AbilitySystem/LokiAbilityTypes.h"

FGameplayEffectContext* ULokiAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FLokiGameplayEffectContext();
}
