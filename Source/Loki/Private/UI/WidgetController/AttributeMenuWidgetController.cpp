// Copyright Ludens Studio


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/LokiAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const ULokiAttributeSet* LokiAttributeSet = CastChecked<ULokiAttributeSet>(AttributeSet);
	for (auto& Pair : LokiAttributeSet->TagsToAttributeGetters)
	{
		FGameplayTag Tag = Pair.Key;
		const FGameplayAttribute Attribute = Pair.Value();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda([this, Tag](const FOnAttributeChangeData& Data)
		{
			FLokiAttributeInfo Info = AttributeInfo->FindAttributeByTag(Tag, true);
			Info.AttributeValue = Data.NewValue;
			OnAttributeInfoChanged.Broadcast(Info);
		});
	}
}

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	const ULokiAttributeSet* LokiAttributeSet = CastChecked<ULokiAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : LokiAttributeSet->TagsToAttributeGetters)
	{
		FLokiAttributeInfo Info = AttributeInfo->FindAttributeByTag(Pair.Key, true);
		Info.AttributeValue = Pair.Value().GetNumericValue(LokiAttributeSet);
		OnAttributeInfoChanged.Broadcast(Info);
	}
}
