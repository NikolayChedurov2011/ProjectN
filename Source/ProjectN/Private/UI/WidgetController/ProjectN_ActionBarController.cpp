// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_ActionBarController.h"

#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_InventoryComponent.h"
#include "UI/Widgets/Containers/ProjectN_ActionBartWidget.h"
#include "UI/Widgets/Slots/ActionSlot/ProjectN_ActionSlot.h"

void UProjectN_ActionBarController::BindCallbacksToResponce()
{
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent);
	UProjectN_InventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (!InventoryComponent || !ProjectN_AbilitySystemComponent)
	{
		return;
	}
	
	ProjectN_AbilitySystemComponent->InputTagTriggered.BindLambda([this, InventoryComponent, ProjectN_AbilitySystemComponent](const FGameplayTag InputTag)
	{
		const UProjectN_ActionSlot* ActionSlot = ActionBarWidget->ActionSlot(InputTag);

		if (!ActionSlot)
		{
			return;
		}
		
		if (ActionSlot->GetItemID().IsNone())
		{
			return;
		}
		
		const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ActionSlot->GetItemID());
			
		if (!EntriesDefinition)
		{
			return;
		}
			
		const FTypeFragment* TypeFragment = GetFragment<FTypeFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Type);
		
		if (TypeFragment->GetEntryType() == EEntryType::Ability)
		{
			const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
			
			ProjectN_AbilitySystemComponent->ServerTryActivateActionBarAbility(AbilityFragment->GetAbilityClass(), AbilityFragment->GetCooldownTag());
		}
		else
		{
			InventoryComponent->ServerTryUseItem(ActionSlot->GetItemID());
		}
	});
}

void UProjectN_ActionBarController::BroadcastInitialValues()
{
	// TODO: Load saved action bar from save

}

void UProjectN_ActionBarController::SetActionBarWidgetRef(UProjectN_ActionBartWidget* NewActionBarWidget)
{
	ActionBarWidget = NewActionBarWidget;
}

void UProjectN_ActionBarController::UpdateActionSlot(const int32 ActionSlotIndex, const FName& IncomingItemID) const
{
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent);
	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(IncomingItemID);
			
	if (!EntriesDefinition)
	{
		return;
	}

	UProjectN_ActionSlot* ActionSlot = ActionBarWidget->ActionSlot(ActionSlotIndex);
			
	const FIconFragment* IconFragment = GetFragment<FIconFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Icon);
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
			
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(IconFragment->GetIcon());
	ActionSlot->SetItemIcon(NewBrush);
	ActionSlot->SetItemID(IncomingItemID);

	if (AbilityFragment)
	{
		ActionSlot->SetCooldownTag(AbilityFragment->GetCooldownTag());
		ActionSlot->SetCooldownBaseValue(AbilityFragment->GetCooldownValue());

		if (AbilityFragment->GetCost().IsValid())
		{
			ActionSlot->SetCostText(AbilityFragment->GetCost().Get<FLabeledFragment>().GetValue());
		}
		

		ActionSlot->SetCooldownValueRemaining(ProjectN_AbilitySystemComponent->FindCooldownRemaining(AbilityFragment->GetCooldownTag()));
	}
}

void UProjectN_ActionBarController::ClearActionSlot(const int32 ActionSlotIndex) const
{
	ActionBarWidget->ActionSlot(ActionSlotIndex)->ClearSlot();
}

void UProjectN_ActionBarController::SwapActionSlots(const int32 ToSlotIndex, const int32 FromSlotIndex, const FName& IncomingItemID) const
{
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent);
	
	UProjectN_ActionSlot* CurrentActionSlot = ActionBarWidget->ActionSlot(ToSlotIndex);
	UProjectN_ActionSlot* IncomingActionSlot = ActionBarWidget->ActionSlot(FromSlotIndex);

	const FEntriesDefinition* CurrentEntriesDefinition = GetEntryManifest(CurrentActionSlot->GetItemID());
	const FEntriesDefinition* IncomingEntriesDefinition = GetEntryManifest(IncomingItemID);
			
	if (!CurrentEntriesDefinition || !IncomingEntriesDefinition)
	{
		return;
	}
			
	const FIconFragment* CurrentIconFragment = GetFragment<FIconFragment>(*CurrentEntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Icon);
	const FIconFragment* IncomingIconFragment = GetFragment<FIconFragment>(*IncomingEntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Icon);

	const FAbilityFragment* CurrentAbilityFragment = GetFragment<FAbilityFragment>(*CurrentEntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
	const FAbilityFragment* IncomingAbilityFragment = GetFragment<FAbilityFragment>(*IncomingEntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
			
	const FName CurrentItemId = CurrentActionSlot->GetItemID();

	FSlateBrush NewCurrentBrush;
	NewCurrentBrush.SetResourceObject(IncomingIconFragment->GetIcon());
	CurrentActionSlot->SetItemIcon(NewCurrentBrush);
	CurrentActionSlot->SetItemID(IncomingItemID);

	FSlateBrush NewIncomingBrush;
	NewIncomingBrush.SetResourceObject(CurrentIconFragment->GetIcon());
	IncomingActionSlot->SetItemIcon(NewIncomingBrush);
	IncomingActionSlot->SetItemID(CurrentItemId);

	if (CurrentAbilityFragment)
	{
		IncomingActionSlot->SetCooldownTag(CurrentAbilityFragment->GetCooldownTag());
		IncomingActionSlot->SetCooldownBaseValue(CurrentAbilityFragment->GetCooldownValue());

		if (CurrentAbilityFragment->GetCost().IsValid())
		{
			IncomingActionSlot->SetCostText(CurrentAbilityFragment->GetCost().Get<FLabeledFragment>().GetValue());
		}

		IncomingActionSlot->SetCooldownValueRemaining(ProjectN_AbilitySystemComponent->FindCooldownRemaining(CurrentAbilityFragment->GetCooldownTag()));
	}

	if (IncomingAbilityFragment)
	{
		CurrentActionSlot->SetCooldownTag(IncomingAbilityFragment->GetCooldownTag());
		CurrentActionSlot->SetCooldownBaseValue(IncomingAbilityFragment->GetCooldownValue());

		if (IncomingAbilityFragment->GetCost().IsValid())
		{
			CurrentActionSlot->SetCostText(IncomingAbilityFragment->GetCost().Get<FLabeledFragment>().GetValue());
		}

		CurrentActionSlot->SetCooldownValueRemaining(ProjectN_AbilitySystemComponent->FindCooldownRemaining(IncomingAbilityFragment->GetCooldownTag()));
	}
}

/*******************
*   Getters
********************/
FEntriesDefinition* UProjectN_ActionBarController::GetEntryManifest(const FName& ItemID) const
{
	if (!Entries) return nullptr;
	
	const FString Context = FString(TEXT("UProjectN_ActionBarController::FindEntryFromDataTable"));
	
	if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(ItemID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}