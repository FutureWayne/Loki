// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "LokiCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

UCLASS()
class LOKI_API ALokiCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALokiCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() {}
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;

	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities() const;

	virtual FVector GetCombatSocketLocation() override;

	virtual FVector GetCombatAimLocation() override;

	virtual void UpdateFacingTarget(const FVector& TargetLocation) override;

	virtual void Die() override;

	/** Combat Interface */

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	/** Ability System */

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Loki|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Loki|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Loki|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

private:
	UPROPERTY(EditAnywhere, Category = "Loki|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
