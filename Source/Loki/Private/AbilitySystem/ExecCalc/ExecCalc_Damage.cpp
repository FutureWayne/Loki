// Copyright Ludens Studio


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/LokiAbilitySystemLibrary.h"
#include "AbilitySystem/LokiAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Singleton/LokiGameplayTags.h"

struct LokiDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	
	LokiDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULokiAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULokiAttributeSet, ArmorPenetration, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULokiAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULokiAttributeSet, CriticalHitChance, Source, true)
	}
};

static const LokiDamageStatics& DamageStatics()
{
	static LokiDamageStatics DmgStatics;
	return DmgStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);

	check(TargetCombatInterface);
	check(SourceCombatInterface);

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	const FGameplayEffectContextHandle EffectContext = Spec.GetContext();

	// Get Damage Set by caller magnitude
	float Damage = 0.f;
	FGameplayTag DamageTypesTag = LokiGameplayTags::Damage;
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	// This container will hold all child tags of the given parent tag
	FGameplayTagContainer ChildTags;

	// Get all children of the ParentTag; the second parameter is a boolean that determines whether to include the ParentTag itself in the list
	ChildTags = TagsManager.RequestGameplayTagChildren(DamageTypesTag);
	for (FGameplayTag DamageTypeTag : ChildTags)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
		Damage += DamageTypeValue;
	}

	// If block, halve the damage
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) <= TargetBlockChance;
	Damage = bBlocked ? Damage / 2.f : Damage;
	ULokiAbilitySystemLibrary::SetBlockedHit(EffectContext, bBlocked);

	// If critical hit, double the damage
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance);

	const bool bCriticalHit = FMath::RandRange(1, 100) <= SourceCriticalHitChance;
	Damage = bCriticalHit ? Damage * 2.f : Damage;
	ULokiAbilitySystemLibrary::SetCriticalHit(EffectContext, bCriticalHit);

	// Armor ignores a percentage of the incoming damage
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);
	
	// Armor penetration ignores a percentage of the target's armor
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	const UCharacterClassInfo* CharacterClassInfo = ULokiAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCharacterLevel());

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetCharacterLevel());

	float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	EffectiveArmor = FMath::Max(0.f, EffectiveArmor);
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(ULokiAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
