// Copyright Ludens Studio


#include "AbilitySystem/Abilities/LokiDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

void ULokiDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	for (auto Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		DamageSpecHandle.Data.Get()->SetSetByCallerMagnitude(Pair.Key, ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage ULokiDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& Montages)
{
	if (Montages.Num() > 0)
	{
		FTaggedMontage Montage = Montages[FMath::RandRange(0, Montages.Num() - 1)];
		// Print montage tag and montage name
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Montage: %s"), *Montage.MontageTag.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Montage: %s"), *Montage.Montage->GetName()));
		return Montage;
	}

	return FTaggedMontage();
}

FTaggedMontage ULokiDamageGameplayAbility::GetLoopedTaggedMontageFromArray(const TArray<FTaggedMontage>& Montages)
{
	if (Montages.Num() > 0)
	{
		Index = (Index + 1) % Montages.Num();
		FTaggedMontage Montage= Montages[Index];

		// Print montage tag and montage name
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Montage: %s"), *Montage.MontageTag.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Montage: %s"), *Montage.Montage->GetName()));
		return Montage;
	}
	return FTaggedMontage();
}
