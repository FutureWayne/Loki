// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Character/LokiCharacterBase.h"
#include "LokiPlayer.generated.h"

class UMotionWarpingComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class LOKI_API ALokiPlayer : public ALokiCharacterBase
{
	GENERATED_BODY()

public:
	ALokiPlayer();
	
	virtual void PossessedBy(AController* NewController) override;

	/** Combat Interface */
	virtual int32 GetCharacterLevel() override;

protected:
	virtual void InitAbilityActorInfo() override;

	virtual FVector GetCombatAimLocation() override;

	virtual void UpdateFacingTarget(const FVector& TargetLocation) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	UMotionWarpingComponent* MotionWarpingComponent;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};
