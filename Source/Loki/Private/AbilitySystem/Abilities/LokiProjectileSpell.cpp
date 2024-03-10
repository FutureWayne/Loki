// Copyright Ludens Studio

#include "AbilitySystem/Abilities/LokiProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/LokiProjectile.h"
#include "Components/SphereComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Singleton/LokiGameplayTags.h"

void ULokiProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void ULokiProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), FLokiGameplayTags::Get().Montage_Attack_Weapon);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		ICombatInterface::Execute_UpdateFacingTarget(GetAvatarActorFromActorInfo(), ProjectileTargetLocation);
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		if (ALokiProjectile* Projectile = GetWorld()->SpawnActorDeferred<ALokiProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			// Give the Projectile a Gameplay Effect Spec Handle for causing Damage.
			const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);
			
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

			const FLokiGameplayTags GameplayTags = FLokiGameplayTags::Get();

			for (auto& Pair : DamageTypes)
			{
				const FGameplayTag DamageTypeTag = Pair.Key;
				const float ScaledDamageValue = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, ScaledDamageValue);
			}
			
			Projectile->DamageEffectSpecHandle = SpecHandle;
			Projectile->SphereComponent->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);

			// Complete the spawn after setting up initial conditions
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
