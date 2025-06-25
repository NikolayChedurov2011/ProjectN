// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"

#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpDataInfo.h"

void UProjectN_OverlayWidgetController::BroadcastInitialValues()
{
	if (!IsValid(AttributeSet) || !IsValid(AbilitySystemComponent))
	{
		return;
	}
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);
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

	if (UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ProjectN_AbilitySystemComponent->bIsAbilityAdded)
		{
			OnInitializeAbility(ProjectN_AbilitySystemComponent);
		}
		else
		{
			ProjectN_AbilitySystemComponent->AbilitiesGiven.AddUObject(this, &UProjectN_OverlayWidgetController::OnInitializeAbility);
		}
		
		ProjectN_AbilitySystemComponent->EffectAssetTags.AddLambda([this] (const FGameplayTagContainer& EffectAssetTags)
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

		ProjectN_AbilitySystemComponent->OnNewCooldown.BindLambda([this] (const FGameplayTag& CooldownTag, const float CooldownRemaining)
		{
			if (CooldownChange.IsBound())
			{
				CooldownChange.Broadcast(CooldownTag, CooldownRemaining);
			}
		});
	}

	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	ProjectNPlayerState->OnXPChanged.AddUObject(this, &UProjectN_OverlayWidgetController::ProcessXP);
	ProjectNPlayerState->OnLevelChanged.AddLambda([this] (const int32 Value)
	{
		OnLevelChanged.Broadcast(static_cast<float>(Value));
	});
}

void UProjectN_OverlayWidgetController::ProcessXP(const int32 Value) const
{
	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	
	const int32 CurrentLevel = ProjectNPlayerState->GetLevelByXP(Value);
	const int32 MaxLevel = ProjectNPlayerState->GetLevelUpInformationContainer().Num();

	if (CurrentLevel <= MaxLevel && CurrentLevel > 0)
	{
		const int32 PastRequirement = ProjectNPlayerState->GetLevelUpInformationContainer()[CurrentLevel - 1].XPForLevelUp;
		const int32 DeltaRequirement = ProjectNPlayerState->GetXPForNextLevelUpByLevel(CurrentLevel);

		const int32 XPForThisLevel = Value - PastRequirement;
		const float Percent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaRequirement);

		OnXPChanged.Broadcast(Percent);
	}
}

float UProjectN_OverlayWidgetController::CheckCooldownRemainig(const FGameplayTag& CooldownTag) const
{
	if (const UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent))
	{
		return ProjectN_AbilitySystemComponent->FindCooldownRemaining(CooldownTag);
	}
	return 0.f;
}

void UProjectN_OverlayWidgetController::BindGameplayAttributeValueChange(const FGameplayAttribute& AttributeData, const FOnAttributeChangedSignature& OnAttributeChangedDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeData).AddLambda([&OnAttributeChangedDelegate] (const FOnAttributeChangeData& Data)
	{
		OnAttributeChangedDelegate.Broadcast(Data.NewValue, Data.OldValue);
	});
}

void UProjectN_OverlayWidgetController::OnInitializeAbility(UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent) const
{
	/*if (!ProjectN_AbilitySystemComponent->bIsAbilityAdded)
	{
		return;
	}

	FForEachAbilitySignature BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, ProjectN_AbilitySystemComponent] (const FGameplayAbilitySpec& AbilitySpec)
	{
		FProjectNActionSlotInfo* AbilityInfo = &AbilityInfoDataAsset->GetAbilityInfoByIndex();
		if (AbilityInfo != nullptr)
		{
			AbilityInfo->InputTag = ProjectN_AbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);

			if (OnAbilityInfo.IsBound())
			{
				OnAbilityInfo.Broadcast(*AbilityInfo);
			}
		}
	});

	ProjectN_AbilitySystemComponent->ForEachAbility(BroadcastDelegate);*/
}

UItemManifest* UProjectN_OverlayWidgetController::FindManifest(const FName& EntryID) const
{
	return GetEntryManifest(EntryID)? GetEntryManifest(EntryID)->FragmentManifest : nullptr;
}

/*******************
*   Getters
********************/
FEntriesDefinition* UProjectN_OverlayWidgetController::GetEntryManifest(const FName& ItemID) const
{
	if (!Entries) return nullptr;
	
	const FString Context = FString(TEXT("UProjectN_InventoryController::FindEntryFromDataTable"));
	
	if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(ItemID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}