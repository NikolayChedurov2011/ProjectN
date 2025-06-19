// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_ActionBarController.h"

#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_InventoryComponent.h"
#include "UI/Widgets/Inventory/ProjectN_ActionBartWidget.h"
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
		ActionSlot->SetCostText(AbilityFragment->GetCostValue());

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
		IncomingActionSlot->SetCostText(CurrentAbilityFragment->GetCostValue());

		IncomingActionSlot->SetCooldownValueRemaining(ProjectN_AbilitySystemComponent->FindCooldownRemaining(CurrentAbilityFragment->GetCooldownTag()));
	}

	if (IncomingAbilityFragment)
	{
		CurrentActionSlot->SetCooldownTag(IncomingAbilityFragment->GetCooldownTag());
		CurrentActionSlot->SetCooldownBaseValue(IncomingAbilityFragment->GetCooldownValue());
		CurrentActionSlot->SetCostText(IncomingAbilityFragment->GetCostValue());

		CurrentActionSlot->SetCooldownValueRemaining(ProjectN_AbilitySystemComponent->FindCooldownRemaining(IncomingAbilityFragment->GetCooldownTag()));
	}
}


/*
void UProjectN_ActionBarController::AddSlot(FActionSlotData ActionSlotInfo)
{
	if (ActionSlotInfo.ItemIcon == nullptr)
	{
		LoadItemIcon(ActionSlotInfo);
	}
	if (ActionSlotInfo.CooldownTag == FGameplayTag())
	{
		FindCooldownTagFromStruct(ActionSlotInfo);
	}
	
	//AddAbility(ActionSlotInfo);

	ActionSlots.Add(ActionSlotInfo);
	
	if (OnUpdateActionSlot.IsBound())
	{
		OnUpdateActionSlot.Broadcast(ActionSlotInfo);
	}
}

void UProjectN_ActionBarController::ClearSlot(const int32 SlotIndex)
{
	for (int32 i = 0; i < ActionSlots.Num(); i++)
	{
		if (ActionSlots[i].ActionSlotIndex == SlotIndex)
		{
			ActionSlots.RemoveAt(i);
			//RemoveAbility(*ActionSlotsTagDependency.Find(ActionSlots[i].ActionSlotIndex));
			//ActionSlots[i].EntryType = EEntryType::None;
			//ActionSlots[i].ItemIcon = nullptr;
			//ActionSlots[i].ItemID = NAME_None;
			//ActionSlots[i].CooldownTag = FGameplayTag();

			if (OnUpdateActionSlot.IsBound())
			{
				FActionSlotData EmptySlotInfo;
				EmptySlotInfo.ActionSlotIndex = SlotIndex;
				OnUpdateActionSlot.Broadcast(EmptySlotInfo);
			}
		}
	}
}

void UProjectN_ActionBarController::AddAbility(const FActionSlotData& ActionSlotInfo) const
{
	if (ActionSlotInfo.EntryType == EEntryType::None || ActionSlotInfo.ItemID == NAME_None)
	{
		return;
	}
	
	if (UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent))
	{
		switch (ActionSlotInfo.EntryType)
		{
		case EEntryType::Ability :
			if (const FAbilityDefinition* AbilityDef = GetAbilityData(ActionSlotInfo.ItemID))
			{
				ProjectN_AbilitySystemComponent->ServerAddAbility(AbilityDef->Ability, *ActionSlotsTagDependency.Find(ActionSlotInfo.ActionSlotIndex), AbilityDef->CooldownData.Tag);
				return;
			}
		case EEntryType::Item :
		//	if (const FItemDefinition* ItemDef = GetItemData(ActionSlotInfo.ItemID))
			{
		//		ProjectN_AbilitySystemComponent->ServerAddAbility(ItemDef->UseItemAbility, *ActionSlotsTagDependency.Find(ActionSlotInfo.ActionSlotIndex));
				return;
			}
		case EEntryType::Equipment :
			//if (const FEquippableItemDefinition* ItemDef = GetEquippableItemData(ActionSlotInfo.ItemID))
			{
				return;
			}
		case EEntryType::Weapon :
			//if (const FWeaponItemDefinition* ItemDef = GetWeaponData(ActionSlotInfo.ItemID))
			{
				return;
			}
		case EEntryType::Bag :
			//if (const FBagDefinition* ItemDef = GetBagData(ActionSlotInfo.ItemID))
			{
				return;
			}
		default: ;
		}
	}	
}

void UProjectN_ActionBarController::RemoveAbility(const FGameplayTag InputActionTag) const
{
	if (UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent))
	{
		ProjectN_AbilitySystemComponent->ServerRemoveAbility(InputActionTag);
	}	
}

void UProjectN_ActionBarController::LoadItemIcon(FActionSlotData& ActionSlotInfo) const
{
	switch (ActionSlotInfo.EntryType)
	{
	case EEntryType::Ability :
		if (const FAbilityDefinition* ItemDef = GetAbilityData(ActionSlotInfo.ItemID))
		{
			ActionSlotInfo.ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::Item :
		if (const FItemDefinition* ItemDef = GetItemData(ActionSlotInfo.ItemID))
		{
			ActionSlotInfo.ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::ConsumableItem :
		if (const FConsumableItemDefinition* ConsumableItemDef = GetConsumableItemData(ActionSlotInfo.ItemID))
		{
			ActionSlotInfo.ItemIcon = ConsumableItemDef->ItemIcon;
			return;
		}
	case EEntryType::Equipment :
		if (const FEquippableItemDefinition* ItemDef = GetEquippableItemData(ActionSlotInfo.ItemID))
		{
			ActionSlotInfo.ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::Weapon :
		if (const FWeaponItemDefinition* ItemDef = GetWeaponData(ActionSlotInfo.ItemID))
		{
			ActionSlotInfo.ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::Bag :
		if (const FBagDefinition* ItemDef = GetBagData(ActionSlotInfo.ItemID))
		{
			ActionSlotInfo.ItemIcon = ItemDef->ItemIcon;
			return;
		}
	default: ;
	}
}

void UProjectN_ActionBarController::FindCooldownTagFromStruct(FActionSlotData& InventorySlotData) const
{
	switch (InventorySlotData.EntryType)
	{
	case EEntryType::Ability :
		if (const FAbilityDefinition* AbilityDef = GetAbilityData(InventorySlotData.ItemID))
		{
			InventorySlotData.CooldownTag = AbilityDef->CooldownData.Tag;
			return;
		}
	case EEntryType::ConsumableItem :
		if (const FConsumableItemDefinition* ConsumableItemDef = GetConsumableItemData(InventorySlotData.ItemID))
		{
			InventorySlotData.CooldownTag = ConsumableItemDef->CooldownData.Tag;
			return;
		}
	default: ;
	}
}
*/


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