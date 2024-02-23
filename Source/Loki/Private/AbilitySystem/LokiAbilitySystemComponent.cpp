// Copyright Ludens Studio


#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/LokiGameplayAbility.h"

void ULokiAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ULokiAbilitySystemComponent::EffectApplied);
}

void ULokiAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			if (const ULokiGameplayAbility* LokiAbility = Cast<ULokiGameplayAbility>(AbilitySpec.Ability))
			{
				AbilitySpec.DynamicAbilityTags.AddTag(LokiAbility->StartupInputTag);
				GiveAbility(AbilitySpec);
			}
		}
	}
}

void ULokiAbilitySystemComponent::AbilityTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void ULokiAbilitySystemComponent::AbilityTagPressed(const FGameplayTag& InputTag)
{
}

void ULokiAbilitySystemComponent::AbilityTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void ULokiAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied,
                                                FActiveGameplayEffectHandle ActiveHandle) const
{
	FGameplayTagContainer TagContainer;
	SpecApplied.GetAllAssetTags(TagContainer);
	OnEffectAssetTags.Broadcast(TagContainer);
}
