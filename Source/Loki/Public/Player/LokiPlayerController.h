// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "LokiPlayerController.generated.h"

class UNiagaraSystem;
class USplineComponent;
class IHighlightInterface;
class UDamageTextComponent;
class ULokiAbilitySystemComponent;
struct FInputActionValue;
class ULokiInputConfig;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class LOKI_API ALokiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALokiPlayerController();
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	ULokiAbilitySystemComponent* GetLokiAbilitySystemComponent();

	UFUNCTION()
	void ShowDamageNumber(const float Damage, ACharacter* TargetCharacter) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<ULokiInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<ULokiAbilitySystemComponent> LokiAbilitySystemComponent;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void SetupInputComponent() override;

private:
	void CursorTrace();
	void AutoMove();

	IHighlightInterface* LastHighlightedActor;
	IHighlightInterface* CurrentHighlightedActor;

	FHitResult CursorHit;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.2f;
	bool bShouldAutoMove = false;
	bool bTargeting = false;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Moving)
	float AutoMoveArrivalDistance = 30.f;

	/** Spline Component For Auto Moving*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Moving, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USplineComponent> Spline;

	/** Damage Text Component */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
