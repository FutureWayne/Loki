// Copyright Ludens Studio


#include "Input/LokiInputConfig.h"

const UInputAction* ULokiInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNouFound) const
{
	for (const FLokiInputAction& AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag.MatchesTag(InputTag))
		{
			return AbilityInputAction.InputAction;
		}
	}

	if (bLogNouFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULokiInputConfig::FindAbilityInputActionByTag: InputTag %s not found"), *InputTag.ToString());
	}

	return nullptr;
}
