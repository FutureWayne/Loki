// Copyright Ludens Studio

#include "AbilitySystem/Abilities/LokiProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/LokiProjectile.h"
#include "Components/SphereComponent.h"
#include "Interaction/CombatInterface.h"
#include "Singleton/LokiGameplayTags.h"

void ULokiProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void ULokiProjectileSpell::SpawnProjectile(const AActor* ProjectileTarget)
{
	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface && IsValid(ProjectileTarget))
	{
		const FVector ProjectileTargetLocation = ProjectileTarget->GetActorLocation();

		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), LokiGameplayTags::Montage_Attack_Weapon);
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
			Projectile->TargetActor = const_cast<AActor*>(ProjectileTarget);

			// Give the Projectile a Gameplay Effect Spec Handle for causing Damage.
			const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);
			
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

			for (auto& Pair : DamageTypes)
			{
				const FGameplayTag DamageTypeTag = Pair.Key;
				const float ScaledDamageValue = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTypeTag, ScaledDamageValue);
			}
			
			Projectile->DamageEffectSpecHandle = SpecHandle;
			Projectile->SphereComponent->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
			Projectile->TeamTag = ICombatInterface::Execute_GetTeamTag(GetAvatarActorFromActorInfo());
			// Complete the spawn after setting up initial conditions
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
