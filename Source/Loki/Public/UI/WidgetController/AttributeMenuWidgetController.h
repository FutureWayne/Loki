// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/LokiWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FLokiAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FLokiAttributeInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LOKI_API UAttributeMenuWidgetController : public ULokiWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	virtual void BroadCastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature OnAttributeInfoChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attributes")
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
