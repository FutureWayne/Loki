// Copyright Ludens Studio


#include "Singleton/LokiAssetManager.h"

#include "Singleton/LokiGameplayTags.h"

ULokiAssetManager& ULokiAssetManager::Get()
{
	check(GEngine);
	ULokiAssetManager* LokiAssetManager = Cast<ULokiAssetManager>(GEngine->AssetManager);
	return *LokiAssetManager;
}

void ULokiAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FLokiGameplayTags::InitializeNativeGameplayTags();
}
