// Copyright Ludens Studio


#include "AbilitySystem/AbilityTask/MoveToLocationAndWaitTargets.h"

#include "AbilitySystem/LokiAbilitySystemLibrary.h"
#include "Player/LokiPlayerController.h"

UMoveToLocationAndWaitTargets::UMoveToLocationAndWaitTargets(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
}

UMoveToLocationAndWaitTargets* UMoveToLocationAndWaitTargets::MoveToLocationAndWaitTargets(
	UGameplayAbility* OwningAbility, const FVector& InLocation, float InRange)
{
	UMoveToLocationAndWaitTargets* TaskObj = NewAbilityTask<UMoveToLocationAndWaitTargets>(OwningAbility);
	TaskObj->TargetLocation = InLocation;
	TaskObj->Range = InRange;

	return TaskObj;
}

void UMoveToLocationAndWaitTargets::Activate()
{
	Super::Activate();

	OwningActor = Ability->GetAvatarActorFromActorInfo();
	SetWaitingOnAvatar();
	
	if (ALokiPlayerController* PC = Cast<ALokiPlayerController>(OwningActor->GetInstigatorController()))
	{
		PC->AutoMoveToLocation(TargetLocation);
	}
}

void UMoveToLocationAndWaitTargets::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (AActor* Target = FindNearestTargetInRange())
	{
		OnFindTargetInRange.Broadcast(Target);
		if (ALokiPlayerController* PC = Cast<ALokiPlayerController>(OwningActor->GetInstigatorController()))
		{
			PC->StopAutoMove();
		}
		EndTask();
	}
}

AActor* UMoveToLocationAndWaitTargets::FindNearestTargetInRange() const
{
	TArray<AActor*> LiveTargets;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwningActor);
	ULokiAbilitySystemLibrary::GetLiveCharactersWithinRadius(OwningActor, LiveTargets, ActorsToIgnore, Range, OwningActor->GetActorLocation());

	AActor* NearestTarget = nullptr;
	float NearestDistance = TNumericLimits<float>::Max();
	
	for (AActor* Actor : LiveTargets)
	{
		if (ULokiAbilitySystemLibrary::IsNotFriendly(OwningActor, Actor))
		{
			const float Distance = (OwningActor->GetActorLocation() - Actor->GetActorLocation()).Size();
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestTarget = Actor;
			}
		}
	}

	return NearestTarget;
}
