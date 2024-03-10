// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LokiGameplayAbility.h"
#include "LokiDamageGameplayAbility.generated.h"

struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class LOKI_API ULokiDamageGameplayAbility : public ULokiGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& Montages);

	UFUNCTION(BlueprintCallable)
	FTaggedMontage GetLoopedTaggedMontageFromArray(const TArray<FTaggedMontage>& Montages);

private:
	int32 Index = -1;
};
