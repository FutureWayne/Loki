// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LokiAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class LOKI_API ULokiAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags OnEffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& StartupAbilities);

	void AbilityTagHeld(const FGameplayTag& InputTag);
	void AbilityTagPressed(const FGameplayTag& InputTag);
	void AbilityTagReleased(const FGameplayTag& InputTag);
	
protected:
	void EffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle) const;
};
