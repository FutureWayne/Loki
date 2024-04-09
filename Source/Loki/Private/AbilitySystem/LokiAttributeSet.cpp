// Copyright Ludens Studio


#include "AbilitySystem/LokiAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/LokiAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LokiPlayerController.h"
#include "Singleton/LokiGameplayTags.h"

ULokiAttributeSet::ULokiAttributeSet()
{
	// Primary Attributes
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Primary_Vigor, GetVigorAttribute);

	// Secondary Attributes
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributeGetters.Add(LokiGameplayTags::Attributes_Secondary_MaxMana, GetMaxManaAttribute);
}

void ULokiAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void ULokiAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		UE_LOG(LogTemp, Warning, TEXT("Change Health On %s, Health: %f"), *EffectProperties.TargetAvatarActor->GetName(), GetHealth());
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(LokiGameplayTags::Effect_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}

			const bool bBlockedHit = ULokiAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
			const bool bCriticalHit = ULokiAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);

			ShowFloatingText(EffectProperties, LocalIncomingDamage, bBlockedHit, bCriticalHit);
		}
	}
}

void ULokiAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& EffectProperties)
{
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() && EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);
		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);
	}
}

void ULokiAttributeSet::ShowFloatingText(const FEffectProperties& EffectProperties, const float Damage, const bool bBlockedHit, const bool bCriticalHit)
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	{
		if (const ALokiPlayerController* LokiPC = Cast<ALokiPlayerController>(
			UGameplayStatics::GetPlayerController(EffectProperties.SourceCharacter, 0)))
		{
			LokiPC->ShowDamageNumber(Damage, EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
		}
		if (const ALokiPlayerController* LokiPC = Cast<ALokiPlayerController>(
			UGameplayStatics::GetPlayerController(EffectProperties.TargetCharacter, 0)))
		{
			LokiPC->ShowDamageNumber(Damage, EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}
