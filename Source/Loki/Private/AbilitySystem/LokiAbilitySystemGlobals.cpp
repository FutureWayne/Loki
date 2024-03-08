// Copyright Ludens Studio


#include "AbilitySystem/LokiAbilitySystemGlobals.h"

#include "LokiAbilityTypes.h"

FGameplayEffectContext* ULokiAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FLokiGameplayEffectContext();
}
