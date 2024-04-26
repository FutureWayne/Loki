// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LokiMagicCircle.generated.h"

UCLASS()
class LOKI_API ALokiMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	ALokiMagicCircle();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

};
