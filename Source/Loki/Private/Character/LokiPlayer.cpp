// Copyright Ludens Studio

#include "Character/LokiPlayer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/LokiPlayerController.h"
#include "Player/LokiPlayerState.h"
#include "UI/HUD/LokiHUD.h"
#include "MotionWarpingComponent.h"

ALokiPlayer::ALokiPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void ALokiPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

int32 ALokiPlayer::GetCharacterLevel()
{
	const ALokiPlayerState* LokiPlayerState = GetPlayerState<ALokiPlayerState>();
	check(LokiPlayerState);
	return LokiPlayerState->GetPlayerLevel();
}

void ALokiPlayer::InitAbilityActorInfo()
{
	ALokiPlayerState* LokiPlayerState = GetPlayerState<ALokiPlayerState>();
	check(LokiPlayerState);

	LokiPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(LokiPlayerState, this);
	Cast<ULokiAbilitySystemComponent>(LokiPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = LokiPlayerState->GetAbilitySystemComponent();
	AttributeSet = LokiPlayerState->GetAttributeSet();
	
	if (ALokiPlayerController* LokiPlayerController = Cast<ALokiPlayerController>(GetController()))
	{
		if (ALokiHUD* LokiHUD = Cast<ALokiHUD>(LokiPlayerController->GetHUD()))
		{
			LokiHUD->InitOverlay(LokiPlayerController, LokiPlayerController->GetPlayerState<ALokiPlayerState>(), AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

FVector ALokiPlayer::GetCombatAimLocation()
{
	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	const FRotator CameraRotation = FollowCamera->GetComponentRotation();

	FVector Start = CameraLocation;
	FVector End = CameraLocation + (CameraRotation.Vector() * 10000); // 10000 is an arbitrary distance

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the player character in the line trace

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, // Change this to the appropriate collision channel
		CollisionParams
	))
	{
		return HitResult.ImpactPoint;
	}
	return End;
}

void ALokiPlayer::UpdateFacingTarget(const FVector& TargetLocation)
{
	const FName TargetName = FName("FacingTarget");
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TargetName, TargetLocation);
}
