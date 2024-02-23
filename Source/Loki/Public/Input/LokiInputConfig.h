// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LokiInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FLokiInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class LOKI_API ULokiInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNouFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLokiInputAction> AbilityInputActions;
};
