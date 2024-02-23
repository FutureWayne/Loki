// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FLokiAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;
};


/**
 * 
 */
UCLASS()
class LOKI_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FLokiAttributeInfo FindAttributeByTag(const FGameplayTag& Tag, bool bLogNotFound) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLokiAttributeInfo> AttributeInfos;
};
