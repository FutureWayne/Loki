// Copyright Ludens Studio

#pragma once

#include "GameplayEffectTypes.h"
#include "LokiAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FLokiGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}

	void SetCriticalHit(const bool bCriticalHit) {bIsCriticalHit = bCriticalHit;}
	void SetBlockedHit(const bool bBlockedHit) {bIsBlockedHit = bBlockedHit;}

	virtual UScriptStruct* GetScriptStruct() const override { return FLokiGameplayEffectContext::StaticStruct(); }

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsBlockedHit = false;
};