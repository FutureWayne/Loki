// Copyright Ludens Studio


#include "AbilitySystem/LokiAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Character/LokiPlayer.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LokiPlayerController.h"
#include "Singleton/LokiGameplayTags.h"

ULokiAttributeSet::ULokiAttributeSet()
{
	const FLokiGameplayTags& GameplayTags = FLokiGameplayTags::Get();
	
	// Primary Attributes
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	// Secondary Attributes
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributeGetters.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
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
				TagContainer.AddTag(FLokiGameplayTags::Get().Effect_HitReact);
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

			ShowFloatingText(EffectProperties, LocalIncomingDamage);
		}
	}
}

void ULokiAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& EffectProperties)
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

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

void ULokiAttributeSet::ShowFloatingText(const FEffectProperties& EffectProperties, const float Damage)
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	{
		if (ALokiPlayerController* LokiPC = Cast<ALokiPlayerController>(UGameplayStatics::GetPlayerController(EffectProperties.SourceCharacter, 0)))
		{
			LokiPC->ShowDamageNumber(Damage, EffectProperties.TargetCharacter);
		}
	}
}
