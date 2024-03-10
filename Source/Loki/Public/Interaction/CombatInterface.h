// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* Montage = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOKI_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCharacterLevel();
	
	virtual void Die() = 0;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UAnimMontage* GetHitReactMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	TArray<FTaggedMontage> GetAttackMontages();
};
