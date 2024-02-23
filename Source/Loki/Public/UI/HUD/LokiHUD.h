// Copyright Ludens Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LokiHUD.generated.h"

class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;
class ULokiUserWidget;
class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class LOKI_API ALokiHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet);

private:
	
	/*
	 *Overlay Widget
	 */
	
	UPROPERTY()
	TObjectPtr<ULokiUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULokiUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	/*
	 *Attribute Menu Widget
	 */

	UPROPERTY()
	TObjectPtr<ULokiUserWidget> AttributeMenuWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULokiUserWidget> AttributeMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
};
