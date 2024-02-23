// Copyright Ludens Studio


#include "UI/Widget/LokiUserWidget.h"

void ULokiUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
