// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LokiGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LOKI_API ULokiGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool CanBeCancelledByOtherAbilities() const { return bCanBeCancelledByOtherAbilities; }

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetCanBeCancelledByOtherAbilities(const bool bCanBeCancelled) { bCanBeCancelledByOtherAbilities = bCanBeCancelled; }

private:
	bool bCanBeCancelledByOtherAbilities = false;
};
