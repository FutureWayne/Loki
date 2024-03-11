// Copyright Ludens Studio


#include "AbilitySystem/LokiAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/LokiAbilityTypes.h"
#include "Game/LokiGameModeBase.h"
#include "Interaction/CombatInterface.h"
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
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);

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
                                                     UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities = CharacterClassInfo->CommonAbilities;
	for (const TSubclassOf<UGameplayAbility> CommonAbility : CommonAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CommonAbility, 1, 0));
	}

	const FCharacterClassDefaultInfo& CharacterClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	for (const auto AbilityClass : CharacterClassDefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetCharacterLevel(), 0);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
	
}

UCharacterClassInfo* ULokiAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ALokiGameModeBase* LokiGameModeBase = Cast<ALokiGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LokiGameModeBase == nullptr)
	{
		return nullptr;
	}

	return LokiGameModeBase->CharacterClassInfo;
}

bool ULokiAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLokiGameplayEffectContext* LokiGameplayEffectContext = static_cast<const FLokiGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LokiGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool ULokiAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLokiGameplayEffectContext* LokiGameplayEffectContext = static_cast<const FLokiGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LokiGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void ULokiAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle EffectContextHandle, const bool bBlockedHit)
{
	if (FLokiGameplayEffectContext* LokiGameplayEffectContext = static_cast<FLokiGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LokiGameplayEffectContext->SetBlockedHit(bBlockedHit);
	}
}

void ULokiAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle EffectContextHandle, const bool bCriticalHit)
{
	if (FLokiGameplayEffectContext* LokiGameplayEffectContext = static_cast<FLokiGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LokiGameplayEffectContext->SetCriticalHit(bCriticalHit);
	}
}

void ULokiAbilitySystemLibrary::GetLiveCharactersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), CollisionQueryParams);
		
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.Add(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool ULokiAbilitySystemLibrary::IsNotFriendly(const AActor* ActorA, const AActor* ActorB)
{
	return (ActorA->ActorHasTag(FName("Player")) && ActorB->ActorHasTag(FName("Enemy"))) || 
	   (ActorA->ActorHasTag(FName("Enemy")) && ActorB->ActorHasTag(FName("Player")));
}
