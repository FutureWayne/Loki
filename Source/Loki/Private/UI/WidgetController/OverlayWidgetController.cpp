// Copyright Ludens Studio


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "AbilitySystem/LokiAttributeSet.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	Super::BroadCastInitialValues();

	const ULokiAttributeSet* LokiAttributeSet = CastChecked<ULokiAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(LokiAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(LokiAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(LokiAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(LokiAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const ULokiAttributeSet* LokiAttributeSet = CastChecked<ULokiAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LokiAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
		OnHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LokiAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LokiAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LokiAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});

	Cast<ULokiAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTags.AddLambda([this](const FGameplayTagContainer& Tags) {
		for (const FGameplayTag& Tag : Tags)
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
	});
}
