// Copyright Ludens Studio


#include "AbilitySystem/AbilityTask/MoveToTargetAndWaitEnterRange.h"

#include "Player/LokiPlayerController.h"

UMoveToTargetAndWaitEnterRange::UMoveToTargetAndWaitEnterRange(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

UMoveToTargetAndWaitEnterRange* UMoveToTargetAndWaitEnterRange::WaitTargetEnterRange(UGameplayAbility* OwningAbility, AActor* InTarget,
                                                                   const float InRange)
{
	UMoveToTargetAndWaitEnterRange* TaskObj = NewAbilityTask<UMoveToTargetAndWaitEnterRange>(OwningAbility);
	TaskObj->TargetActor = InTarget;
	TaskObj->Range = InRange;

	return TaskObj;
}

void UMoveToTargetAndWaitEnterRange::Activate()
{
	Super::Activate();
	OwningActor = Ability->GetAvatarActorFromActorInfo();

	SetWaitingOnAvatar();
}

void UMoveToTargetAndWaitEnterRange::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (IsTargetWithinRange())
	{
		OnTargetEnterRange.Broadcast();
		if (ALokiPlayerController* PC = Cast<ALokiPlayerController>(OwningActor->GetInstigatorController()))
		{
			PC->StopAutoMove();
		}
		EndTask();
	}
	else
	{
		if (ALokiPlayerController* PC = Cast<ALokiPlayerController>(OwningActor->GetInstigatorController()))
		{
			PC->AutoMoveToLocation(TargetActor->GetActorLocation());
		}
	}
}

bool UMoveToTargetAndWaitEnterRange::IsTargetWithinRange() const
{
	if (OwningActor && IsValid(TargetActor))
	{
		const float Distance = (OwningActor->GetActorLocation() - TargetActor->GetActorLocation()).Size();
		return Distance <= Range;
	}
	return false;
}
