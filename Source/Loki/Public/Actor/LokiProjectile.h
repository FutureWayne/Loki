// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "LokiProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class LOKI_API ALokiProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALokiProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	bool bIsHoming = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float HomingAcceleration = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	bool bCanPenetrate = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	FGameplayTag TeamTag;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Properties")
	AActor* TargetActor = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
};
