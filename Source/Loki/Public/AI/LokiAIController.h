// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LokiAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class LOKI_API ALokiAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALokiAIController();
	
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
