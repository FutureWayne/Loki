// Copyright Ludens Studio


#include "Player/LokiPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Input/LokiInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"


void ALokiPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (GetLokiAbilitySystemComponent() == nullptr)
	{
		return;
	}

	GetLokiAbilitySystemComponent()->AbilityTagPressed(InputTag);
}

void ALokiPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (GetLokiAbilitySystemComponent() == nullptr)
	{
		return;
	}

	GetLokiAbilitySystemComponent()->AbilityTagReleased(InputTag);
}

void ALokiPlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (GetLokiAbilitySystemComponent() == nullptr)
	{
		return;
	}

	GetLokiAbilitySystemComponent()->AbilityTagHeld(InputTag);
}

ULokiAbilitySystemComponent* ALokiPlayerController::GetLokiAbilitySystemComponent()
{
	if (!LokiAbilitySystemComponent)
	{
		LokiAbilitySystemComponent = CastChecked<ULokiAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return LokiAbilitySystemComponent;
}

void ALokiPlayerController::ShowDamageNumber(const float Damage, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		if (UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass))
		{
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageText->SetDamageText(Damage);
		}
	}
}


void ALokiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultMappingContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ALokiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ULokiInputComponent* LokiInputComponent = CastChecked<ULokiInputComponent>(InputComponent);
	check(LokiInputComponent);

	// Jumping
	LokiInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALokiPlayerController::Jump);
	LokiInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ALokiPlayerController::StopJumping);
	
	// Moving
	LokiInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALokiPlayerController::Move);

	// Looking
	LokiInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALokiPlayerController::Look);

	// Ability Actions
	LokiInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ALokiPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALokiPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ALokiPlayerController::Jump(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Jump"));
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void ALokiPlayerController::StopJumping(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Stop Jumping"));
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}


