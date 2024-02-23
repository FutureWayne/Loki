// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LokiGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class LOKI_API ALokiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
