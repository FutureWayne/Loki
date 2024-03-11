// Copyright Ludens Studio


#include "Player/LokiPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Input/LokiInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/LokiGameplayAbility.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/HighlightInterface.h"
#include "Singleton/LokiGameplayTags.h"
#include "UI/Widget/DamageTextComponent.h"


ALokiPlayerController::ALokiPlayerController()
{
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ALokiPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	bTargeting = CurrentHighlightedActor ? true : false;

	// Cancel Abilities if tag for input cancelling
	if (GetLokiAbilitySystemComponent())
	{
		GetLokiAbilitySystemComponent()->CancelAbilities(&InputCancelAbilityTags);
	}

	// Floor Attack
	if (InputTag.MatchesTagExact(FLokiGameplayTags::Get().InputTag_LMB))
	{
		if (bIsFloorAttackAiming)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, AttackFXCursor, CursorHit.ImpactPoint, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
			GetLokiAbilitySystemComponent()->AbilityTagHeld(FLokiGameplayTags::Get().InputTag_RMB);
		}
		else
		{
			// TODO: Enemy Selection
		}
	}

	else if (IsMovementInput(InputTag))
	{
		if (CursorHit.bBlockingHit)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MovementFXCursor, CursorHit.ImpactPoint, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
			AutoMoveToLocation(CursorHit.ImpactPoint);
		}
	}
	
	else if (GetLokiAbilitySystemComponent())
	{
		GetLokiAbilitySystemComponent()->AbilityTagPressed(InputTag);
	}

	FloorAttackCompleteOrCancelled();
}

void ALokiPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FLokiGameplayTags::Get().InputTag_RMB) || bTargeting)
	{
		if (GetLokiAbilitySystemComponent())
		{
			GetLokiAbilitySystemComponent()->AbilityTagReleased(InputTag);
		}
	}
}

void ALokiPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FLokiGameplayTags::Get().InputTag_RMB) || bTargeting)
	{
		if (GetLokiAbilitySystemComponent())
		{
			GetLokiAbilitySystemComponent()->AbilityTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		APawn* ControlledPawn = GetPawn<APawn>();
		if (ControlledPawn && FollowTime >= ShortPressThreshold && CursorHit.bBlockingHit)
		{
			bShouldAutoMove = false;
			CachedDestination = CursorHit.ImpactPoint;
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

ULokiAbilitySystemComponent* ALokiPlayerController::GetLokiAbilitySystemComponent()
{
	if (!LokiAbilitySystemComponent)
	{
		LokiAbilitySystemComponent = CastChecked<ULokiAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return LokiAbilitySystemComponent;
}

void ALokiPlayerController::ShowDamageNumber(const float Damage, ACharacter* TargetCharacter, const bool bBlockedHit, const bool bCriticalHit) const
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		if (UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass))
		{
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageText->SetDamageText(Damage, bBlockedHit, bCriticalHit);
		}
	}
}

void ALokiPlayerController::AutoMoveToLocation(const FVector& Destination)
{
	FollowTime = 0.f;
	CachedDestination = Destination;
	const APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn && CachedDestination != FVector::ZeroVector)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetPawn()->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				}
			}
			bShouldAutoMove = true;
		}
	}
}

void ALokiPlayerController::StopAutoMove()
{
	bShouldAutoMove = false;
	Spline->ClearSplinePoints();
}

void ALokiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultMappingContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetHideCursorDuringCapture(false);
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}

void ALokiPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoMove();
}

void ALokiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ULokiInputComponent* LokiInputComponent = CastChecked<ULokiInputComponent>(InputComponent);
	check(LokiInputComponent);

	LokiInputComponent->BindAction(FloorAttackAction, ETriggerEvent::Completed, this, &ThisClass::FloorAttackReady);

	// Ability Actions
	LokiInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ALokiPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastHighlightedActor = CurrentHighlightedActor;

	CurrentHighlightedActor = Cast<IHighlightInterface>(CursorHit.GetActor());
	if (LastHighlightedActor != CurrentHighlightedActor)
	{
		if (LastHighlightedActor)
		{
			LastHighlightedActor->UnHighlightActor();
		}
		if (CurrentHighlightedActor)
		{
			CurrentHighlightedActor->HighlightActor();
		}
	}
}

void ALokiPlayerController::AutoMove()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (bShouldAutoMove)
		{
			const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
			const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
			ControlledPawn->AddMovementInput(Direction);

			const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
			if (DistanceToDestination <= AutoMoveArrivalDistance)
			{
				bShouldAutoMove = false;
			}
		}
		else
		{
			Spline->ClearSplinePoints();
		}
	}
}

void ALokiPlayerController::FloorAttackReady()
{
	bIsFloorAttackAiming = true;
	// TODO: Change Cursor
}

void ALokiPlayerController::FloorAttackCompleteOrCancelled()
{
	bIsFloorAttackAiming = false;
	// TODO: Change Cursor
}

bool ALokiPlayerController::IsFloorAttackInput(const FGameplayTag InputTag) const
{
	return InputTag.MatchesTagExact(FLokiGameplayTags::Get().InputTag_LMB) || bIsFloorAttackAiming;
}

bool ALokiPlayerController::IsMovementInput(const FGameplayTag InputTag) const
{
	return InputTag.MatchesTagExact(FLokiGameplayTags::Get().InputTag_RMB) && !bTargeting;
}


