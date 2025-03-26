// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"

#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

void UProjectN_OverlayWidgetController::BroadcastInitialValues()
{
	if (!IsValid(AttributeSet) || !IsValid(AbilitySystemComponent))
	{
		return;
	}
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	OnMaxHealthChanged.Broadcast(ProjectN_AttributeSet->GetMaxHealth(), ProjectN_AttributeSet->GetMaxHealth());
	OnHealthChanged.Broadcast(ProjectN_AttributeSet->GetHealth(), ProjectN_AttributeSet->GetHealth());
	OnMaxManaChanged.Broadcast(ProjectN_AttributeSet->GetMaxMana(), ProjectN_AttributeSet->GetMaxMana());
	OnManaChanged.Broadcast(ProjectN_AttributeSet->GetMana(), ProjectN_AttributeSet->GetMana());
	OnMaxStaminaChanged.Broadcast(ProjectN_AttributeSet->GetMaxStamina(), ProjectN_AttributeSet->GetMaxStamina());
	OnStaminaChanged.Broadcast(ProjectN_AttributeSet->GetStamina(), ProjectN_AttributeSet->GetStamina());
}

void UProjectN_OverlayWidgetController::BindCallbacksToResponce()
{
	if (!IsValid(AttributeSet) || !IsValid(AbilitySystemComponent))
	{
		return;
	}
	
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	BindGameplayAttributeValueChange(ProjectN_AttributeSet->GetHealthAttribute(), OnHealthChanged);
	BindGameplayAttributeValueChange(ProjectN_AttributeSet->GetMaxHealthAttribute(), OnMaxHealthChanged);
	BindGameplayAttributeValueChange(ProjectN_AttributeSet->GetManaAttribute(), OnManaChanged);
	BindGameplayAttributeValueChange(ProjectN_AttributeSet->GetMaxManaAttribute(), OnMaxManaChanged);
	BindGameplayAttributeValueChange(ProjectN_AttributeSet->GetStaminaAttribute(), OnStaminaChanged);
	BindGameplayAttributeValueChange(ProjectN_AttributeSet->GetMaxStaminaAttribute(), OnMaxStaminaChanged);
	
	Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([this] (const FGameplayTagContainer& EffectAssetTags)
	{
		for (const FGameplayTag& Tag : EffectAssetTags)
		{
			const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
				const FUIWidgetRow* WidgetRow = GetTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				OnMessageRowSignature.Broadcast(*WidgetRow);
			}
		}
	});
}

void UProjectN_OverlayWidgetController::BindGameplayAttributeValueChange(const FGameplayAttribute& AttributeData, const FOnAttributeChangedSignature& OnAttributeChangedDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeData).AddLambda([&OnAttributeChangedDelegate] (const FOnAttributeChangeData& Data)
	{
		OnAttributeChangedDelegate.Broadcast(Data.NewValue, Data.OldValue);
	});
}