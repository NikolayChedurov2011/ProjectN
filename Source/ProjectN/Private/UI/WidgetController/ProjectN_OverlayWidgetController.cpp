// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"

#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

void UProjectN_OverlayWidgetController::BroadcastInitialValues()
{
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	OnMaxHealthChanged.Broadcast(ProjectN_AttributeSet->GetMaxHealth());
	OnHealthChanged.Broadcast(ProjectN_AttributeSet->GetHealth());
	OnMaxManaChanged.Broadcast(ProjectN_AttributeSet->GetMaxMana());
	OnManaChanged.Broadcast(ProjectN_AttributeSet->GetMana());
	OnMaxStaminaChanged.Broadcast(ProjectN_AttributeSet->GetMaxStamina());
	OnStaminaChanged.Broadcast(ProjectN_AttributeSet->GetStamina());
}

void UProjectN_OverlayWidgetController::BindCallbacksToResponce()
{
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetHealthAttribute()).AddUObject(this, &UProjectN_OverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UProjectN_OverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetManaAttribute()).AddUObject(this, &UProjectN_OverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetMaxManaAttribute()).AddUObject(this, &UProjectN_OverlayWidgetController::MaxManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetStaminaAttribute()).AddUObject(this, &UProjectN_OverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &UProjectN_OverlayWidgetController::MaxStaminaChanged);
	Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& EffectAssetTags)
		{
			for (const auto Tag : EffectAssetTags)
			{
				FUIWidgetRow* WidgetRow = GetTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			}
		}
	);
}

void UProjectN_OverlayWidgetController::HealthChanged(const FOnAttributeChangeData& AttributeData) const
{
	OnHealthChanged.Broadcast(AttributeData.NewValue);
}

void UProjectN_OverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& AttributeData) const
{
	OnMaxHealthChanged.Broadcast(AttributeData.NewValue);
}

void UProjectN_OverlayWidgetController::ManaChanged(const FOnAttributeChangeData& AttributeData) const
{
	OnManaChanged.Broadcast(AttributeData.NewValue);
}

void UProjectN_OverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& AttributeData) const
{
	OnMaxManaChanged.Broadcast(AttributeData.NewValue);
}

void UProjectN_OverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& AttributeData) const
{
	OnStaminaChanged.Broadcast(AttributeData.NewValue);
}

void UProjectN_OverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& AttributeData) const
{
	OnMaxStaminaChanged.Broadcast(AttributeData.NewValue);
}
