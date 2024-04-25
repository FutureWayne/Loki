// Copyright Ludens Studio


#include "Actor/LokiProjectile.h"
#include "Character/LokiCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LokiAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Loki/Loki.h"

// Sets default values
ALokiProjectile::ALokiProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ALokiProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovementComponent->bIsHomingProjectile && IsValid(TargetActor))
	{
		ProjectileMovementComponent->HomingTargetComponent = TargetActor->GetRootComponent();
	}

	SetLifeSpan(LifeSpan);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALokiProjectile::OnSphereBeginOverlap);

	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
}

void ALokiProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsValid(TargetActor) || ICombatInterface::Execute_IsDead(TargetActor))
	{
		Destroy();
	}
}

void ALokiProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALokiCharacterBase* TargetCharacter = Cast<ALokiCharacterBase>(OtherActor);
	if (TargetCharacter && TargetCharacter->TeamTag.MatchesTagExact(TeamTag))
	{
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
	if (LoopingSoundComponent && LoopingSoundComponent->IsPlaying())
	{
		LoopingSoundComponent->Stop();
	}

	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}
	
	if (bCanPenetrate)
	{
		// TODO: Implement logic for handling post-penetration of projectile.
	}
	else
	{
		Destroy();
	}
}

