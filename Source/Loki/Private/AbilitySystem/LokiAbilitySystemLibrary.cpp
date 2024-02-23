// Copyright Ludens Studio


#include "AbilitySystem/LokiAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/LokiGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LokiPlayerState.h"
#include "UI/HUD/LokiHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ULokiAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ALokiHUD* LokiHUD = Cast<ALokiHUD>(PlayerController->GetHUD()))
		{
			ALokiPlayerState* LokiPlayerState = PlayerController->GetPlayerState<ALokiPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = LokiPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = LokiPlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, LokiPlayerState, AbilitySystemComponent, AttributeSet);
			return LokiHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* ULokiAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ALokiHUD* LokiHUD = Cast<ALokiHUD>(PlayerController->GetHUD()))
		{
			ALokiPlayerState* LokiPlayerState = PlayerController->GetPlayerState<ALokiPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = LokiPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = LokiPlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, LokiPlayerState, AbilitySystemComponent, AttributeSet);
			return LokiHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void ULokiAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	const ALokiGameModeBase* LokiGameModeBase = Cast<ALokiGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LokiGameModeBase == nullptr)
	{
		return;
	}

	AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = LokiGameModeBase->CharacterClassInfo;
	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassDefaultInfo.PrimaryAttributesGameplayEffect, Level, PrimaryAttributesEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesEffectSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributesGameplayEffect, Level, SecondaryAttributesEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesEffectSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributesGameplayEffect, Level, VitalAttributesEffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesEffectSpecHandle.Data.Get());
}

void ULokiAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* AbilitySystemComponent)
{
	const ALokiGameModeBase* LokiGameModeBase = Cast<ALokiGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LokiGameModeBase == nullptr)
	{
		return;
	}

	const UCharacterClassInfo* CharacterClassInfo = LokiGameModeBase->CharacterClassInfo;
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities = CharacterClassInfo->CommonAbilities;
	for (const TSubclassOf<UGameplayAbility> CommonAbility : CommonAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CommonAbility, 1, 0));
	}
}
