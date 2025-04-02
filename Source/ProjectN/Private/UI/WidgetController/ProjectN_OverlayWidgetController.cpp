// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"

#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/Data/LevelUpDataInfo.h"

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
	
	const AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	OnXPChanged.Broadcast(ProjectNPlayerState->GetXP());
	OnLevelChanged.Broadcast(ProjectNPlayerState->GetCharacterLevel_Internal());
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

	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	ProjectNPlayerState->OnXPChanged.AddLambda([this, ProjectNPlayerState] (const int32 Value)
	{
		const int32 CurrentLevel = ProjectNPlayerState->LevelUpInfo->GetLevelByXP(Value);
		const int32 MaxLevel = ProjectNPlayerState->LevelUpInfo->LevelUpInformationContainer.Num();

		if (CurrentLevel <= MaxLevel && CurrentLevel > 0)
		{
			const int32 PastRequirement = ProjectNPlayerState->LevelUpInfo->LevelUpInformationContainer[CurrentLevel - 1].XPForLevelUp;
			const int32 DeltaRequirement = ProjectNPlayerState->LevelUpInfo->GetXPForLevelUpByLevel(CurrentLevel);

			const int32 XPForThisLevel = Value - PastRequirement;
			const float Percent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaRequirement);

			OnXPChanged.Broadcast(Percent);
		}
	});

	ProjectNPlayerState->OnLevelChanged.AddLambda([this] (const int32 Value)
	{
		OnLevelChanged.Broadcast(Value);
	});
}

void UProjectN_OverlayWidgetController::BindGameplayAttributeValueChange(const FGameplayAttribute& AttributeData, const FOnAttributeChangedSignature& OnAttributeChangedDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeData).AddLambda([&OnAttributeChangedDelegate] (const FOnAttributeChangeData& Data)
	{
		OnAttributeChangedDelegate.Broadcast(Data.NewValue, Data.OldValue);
	});
}