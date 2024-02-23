// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LokiAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LOKI_API ULokiAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static ULokiAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
