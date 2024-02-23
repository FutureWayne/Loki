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

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector FacingTargetLocation = CombatInterface->GetCombatAimLocation();
		CombatInterface->UpdateFacingTarget(FacingTargetLocation);
	}
}

void ULokiProjectileSpell::SpawnProjectile()
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		const FVector AimLocation = CombatInterface->GetCombatAimLocation();
		const FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, AimLocation);
		
		// Create a transform with the muzzle location and the aim rotation
		const FTransform SpawnTransform(AimRotation, SocketLocation);

		if (ALokiProjectile* Projectile = GetWorld()->SpawnActorDeferred<ALokiProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			// Give the Projectile a Gameplay Effect Spec Handle for causing Damage.
			const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

			const FLokiGameplayTags GameplayTags = FLokiGameplayTags::Get();
			const float DamageValue = Damage.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, DamageValue);
			Projectile->DamageEffectSpecHandle = SpecHandle;

			Projectile->SphereComponent->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);

			// Complete the spawn after setting up initial conditions
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
