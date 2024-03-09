// Copyright Ludens Studio

#include "AbilitySystem/LokiAbilityTypes.h"


bool FLokiGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
