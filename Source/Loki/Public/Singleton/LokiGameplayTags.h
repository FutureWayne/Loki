// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Loki Gameplay Tags
 *
 *  Singleton containing native Gameplay Tags
 */

struct FLokiGameplayTags
{
    static const FLokiGameplayTags& Get()
    {
        return GameplayTags;
    }

    static void InitializeNativeGameplayTags();

    FGameplayTag Attributes_Primary_Strength;
    FGameplayTag Attributes_Primary_Intelligence;
    FGameplayTag Attributes_Primary_Resilience;
    FGameplayTag Attributes_Primary_Vigor;

    FGameplayTag Attributes_Secondary_Armor;
    FGameplayTag Attributes_Secondary_ArmorPenetration;
    FGameplayTag Attributes_Secondary_BlockChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_CriticalHitDamage;
    FGameplayTag Attributes_Secondary_CriticalHitResistance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;
    FGameplayTag Attributes_Secondary_MaxHealth;
    FGameplayTag Attributes_Secondary_MaxMana;

    FGameplayTag InputTag_LMB;
    FGameplayTag InputTag_RMB;
    FGameplayTag InputTag_1;
    FGameplayTag InputTag_2;
    FGameplayTag InputTag_3;
    FGameplayTag InputTag_4;

    FGameplayTag Damage;
    FGameplayTag Damage_Fire;
    FGameplayTag Damage_Physical;

    FGameplayTag Effect_HitReact;

    FGameplayTag Abilities_Attack;
    FGameplayTag Abilities_CancelByInput;

    FGameplayTag Montage_Attack_Weapon;
    FGameplayTag Montage_Attack_RightHand;
    FGameplayTag Montage_Attack_LeftHand;

    TArray<FGameplayTag> DamageTypes;

private:
    static FLokiGameplayTags GameplayTags;
 
};
