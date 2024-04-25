// Copyright Ludens Studio


#include "Singleton/LokiGameplayTags.h"

namespace LokiGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Strength, "Attributes.Primary.Strength", "Increases physical damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Resilience, "Attributes.Primary.Resilience", "Increases Armor and Armor Penetration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Vigor, "Attributes.Primary.Vigor", "Increases Health");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces damage taken, improves Block Chance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Ignores Percentage of enemy Armor, increases Critical Hit Chance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance", "Chance to cut incoming damage in half");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", "Reduces Critical Hit Chance of attacking enemies");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Amount of Health regenerated every 1 second");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Amount of Mana regenerated every 1 second");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of Health obtainable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of Mana obtainable");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "Input Tag for Left Mouse Button");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "Input Tag for Right Mouse Button");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_1, "InputTag.1", "Input Tag for 1 key");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_2, "InputTag.2", "Input Tag for 2 key");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_3, "InputTag.3", "Input Tag for 3 key");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_4, "InputTag.4", "Input Tag for 4 key");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Flash, "InputTag.Flash", "Input Tag for Flash");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_FloorAttack, "InputTag.FloorAttack", "Input Tag for Floor Attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Stop, "InputTag.Stop", "Input Tag for Stop");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "Damage", "Damage Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Fire, "Damage.Fire", "Fire Damage Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", "Physical Damage Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_FireBolt, "Cooldown.FireBolt", "Fire Bolt Cooldown Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_HitReact, "Effect.HitReact", "Hit React Effect Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", "Attack Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_CancelByInput, "Abilities.CancelByInput", "Cancel Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_Weapon, "Montage.Attack.Weapon", "Attack Montage Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_RightHand, "Montage.Attack.RightHand", "Right Hand Attack Montage Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_LeftHand, "Montage.Attack.LeftHand", "Left Hand Attack Montage Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Team_Radiant, "Team.Radiant", "Radiant Team Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Team_Dire, "Team.Dire", "Dire Team Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Team_Neutral, "Team.Neutral", "Neutral Team Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Montage_Attack_Melee, "Events.Montage.Attack.Melee", "Melee Attack Montage Event Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Montage_Attack_Range, "Events.Montage.Attack.Range", "Range Attack Montage Event Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Events_Montage_Cast_FireBolt, "Events.Montage.Cast.FireBolt", "Cast Fire Bolt Montage Event Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_HealthPotion, "Message.HealthPotion", "Health Potion Message Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_ManaPotion, "Message.ManaPotion", "Mana Potion Message Tag");
}
