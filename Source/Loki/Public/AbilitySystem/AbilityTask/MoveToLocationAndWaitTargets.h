// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MoveToLocationAndWaitTargets.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindTargetInRange, AActor*, FoundTarget);

/**
 * 
 */
UCLASS()
class LOKI_API UMoveToLocationAndWaitTargets : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FFindTargetInRange OnFindTargetInRange;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "MoveToLocationAndWaitTargets", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UMoveToLocationAndWaitTargets* MoveToLocationAndWaitTargets(UGameplayAbility* OwningAbility, const FVector& InLocation, float InRange);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

private:
	UPROPERTY()
	float Range = 0.f;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	TObjectPtr<AActor> OwningActor;
	
	AActor* FindNearestTargetInRange() const;
};
