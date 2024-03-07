// Copyright Ludens Studio

#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* TaskObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return TaskObj;
}

void UTargetDataUnderMouse::Activate()
{
	const APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	OnTargetDataReady.Broadcast(HitResult.ImpactPoint);
}
