// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "LokiPlayerController.generated.h"

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
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	ULokiAbilitySystemComponent* GetLokiAbilitySystemComponent();

	UFUNCTION()
	void ShowDamageNumber(const float Damage, ACharacter* TargetCharacter);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<ULokiInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<ULokiAbilitySystemComponent> LokiAbilitySystemComponent;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jumping input */
	void Jump(const FInputActionValue& Value);

	/** Called for stopping jumping input */
	void StopJumping(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
