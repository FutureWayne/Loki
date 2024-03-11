// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MoveToTargetAndWaitEnterRange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetEnterRangeDelegate);

/**
 * 
 */
UCLASS()
class LOKI_API UMoveToTargetAndWaitEnterRange : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FTargetEnterRangeDelegate OnTargetEnterRange;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitTargetEnterRange", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UMoveToTargetAndWaitEnterRange* WaitTargetEnterRange(UGameplayAbility* OwningAbility, AActor* InTarget, float InRange);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

private:
	UPROPERTY()
	float Range = 0.f;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	UPROPERTY()
	TObjectPtr<AActor> OwningActor;

	bool IsTargetWithinRange() const;
};
