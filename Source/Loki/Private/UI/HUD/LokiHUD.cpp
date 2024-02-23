// Copyright Ludens Studio


#include "UI/HUD/LokiHUD.h"
#include "UI/Widget/LokiUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* ALokiHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ALokiHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void ALokiHUD::InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState,
                           UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<ULokiUserWidget>(Widget);

	const FWidgetControllerParams WCParams(InPlayerController, InPlayerState, InAbilitySystemComponent, InAttributeSet);
	OverlayWidgetController = GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController);

	OverlayWidgetController->BroadCastInitialValues();

	OverlayWidget->AddToViewport();
}
