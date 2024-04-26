// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "LokiAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class LOKI_API ULokiAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "LokiAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "LokiAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "LokiAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "LokiAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "LokiAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "LokiAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "LokiAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "LokiAbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(FGameplayEffectContextHandle EffectContextHandle, bool bBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "LokiAbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(FGameplayEffectContextHandle EffectContextHandle, bool bCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveCharactersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriendly(const AActor* ActorA, const AActor* ActorB);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static AActor* FindNearestEnemyWithinRadius(const UObject* WorldContextObject, const FVector& SphereOrigin, float Radius, AActor* SourceActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TSet<AActor*> FindEnemiesWithinRadius(const UObject* WorldContextObject, const FVector& SphereOrigin, float Radius, AActor* SourceActor);
};
