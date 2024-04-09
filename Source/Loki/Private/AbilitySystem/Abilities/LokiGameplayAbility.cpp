// Copyright Ludens Studio


#include "AbilitySystem/Abilities/LokiGameplayAbility.h"

#include "Player/LokiPlayerController.h"
#include "Singleton/LokiGameplayTags.h"

void ULokiGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ALokiPlayerController* LokiPlayerController = Cast<ALokiPlayerController>(ActorInfo->PlayerController.Get()))
	{
		LokiPlayerController->OnMouseClickDelegate.RemoveAll(this);
		LokiPlayerController->OnMouseClickDelegate.AddDynamic(this, &ULokiGameplayAbility::OnMouseClick);
	}
}

void ULokiGameplayAbility::ConfirmCurrentAbility_Implementation()
{
}

void ULokiGameplayAbility::CancelCurrentAbility_Implementation()
{
}

void ULokiGameplayAbility::OnMouseClick(const FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(LokiGameplayTags::InputTag_LMB))
	{
		ConfirmCurrentAbility();
	}
	else if (InputTag.MatchesTagExact(LokiGameplayTags::InputTag_RMB))
	{
		CancelCurrentAbility();
	}
}


