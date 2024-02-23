// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LokiAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// A generic type pointer to a static function
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class LOKI_API ULokiAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULokiAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Map of a gameplay tag to a static attribute getter function
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributeGetters;

	/*
	 * Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Mana);
	
	/*
	 * Primary Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Vigor);
	
	/*
	 * Secondary Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, MaxMana);


	/*
	 * Meta Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(ULokiAttributeSet, IncomingDamage);
	
private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties);

	static void ShowFloatingText(const FEffectProperties& EffectProperties, const float Damage);
};
