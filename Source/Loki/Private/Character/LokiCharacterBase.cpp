// Copyright Ludens Studio


#include "Character/LokiCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Loki/Loki.h"
#include "Singleton/LokiGameplayTags.h"

// Sets default values
ALokiCharacterBase::ALokiCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ALokiCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ALokiCharacterBase::GetHitReactMontage_Implementation() const
{
	return HitReactMontage;
}

// Called when the game starts or when spawned
void ALokiCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALokiCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(EffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ALokiCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);
}

void ALokiCharacterBase::AddCharacterAbilities() const
{
	ULokiAbilitySystemComponent* LokiAbilitySystemComponent = CastChecked<ULokiAbilitySystemComponent>(GetAbilitySystemComponent());

	LokiAbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

FVector ALokiCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	FVector SocketLocation = FVector::ZeroVector;
	const FLokiGameplayTags& LokiGameplayTags = FLokiGameplayTags::Get();
	if (MontageTag.MatchesTagExact(LokiGameplayTags.Montage_Attack_Weapon))
	{
		SocketLocation = GetMesh()->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(LokiGameplayTags.Montage_Attack_LeftHand))
	{
		SocketLocation = GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(LokiGameplayTags.Montage_Attack_RightHand))
	{
		SocketLocation = GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	return SocketLocation;
}

void ALokiCharacterBase::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	bDead = true;
}
bool ALokiCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ALokiCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ALokiCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}
