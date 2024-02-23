// Copyright Ludens Studio


#include "Player/LokiPlayerState.h"

#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "AbilitySystem/LokiAttributeSet.h"

ALokiPlayerState::ALokiPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<ULokiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<ULokiAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ALokiPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
