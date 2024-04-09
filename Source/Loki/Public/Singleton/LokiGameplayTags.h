// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace LokiGameplayTags
{
	LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);
    
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);
    
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);
    
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);
    
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_HitReact);
    
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_CancelByInput);

    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_Weapon);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_RightHand);
    LOKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_LeftHand);
}
