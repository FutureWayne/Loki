// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LokiUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOKI_API ULokiUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
	void OnWidgetControllerSet();
};
