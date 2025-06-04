// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_ActionBarController.h"

#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
//#include "GameFramework/PlayerState.h"

void UProjectN_ActionBarController::BindCallbacksToResponce()
{

	/*if (InventoryComponent)
	{
		

		InventoryComponent->OnBagChanged.BindLambda([this, InventoryComponent](const int32 BagID, const int32 BagSlots)
		{
			if (OnUpdateBag.IsBound())
			{
				OnUpdateBag.Broadcast(BagID, BagSlots);
			}
		});

		InventoryComponent->OnSlotChange.BindLambda([this, InventoryComponent](const int32 BagID, const int32 BagSlots, const FInventorySlotData& ItemData)
		{
			if (OnUpdateSlot.IsBound())
			{
				OnUpdateSlot.Broadcast(BagID, BagSlots, ItemData);
			}
		});
	}*/
}

void UProjectN_ActionBarController::BroadcastInitialValues()
{
	// TODO: Load saved action bar from save

}

void UProjectN_ActionBarController::AddSlot(FActionSlotData ActionSlotInfo)
{
	if (ActionSlotInfo.ItemIcon == nullptr)
	{
		LoadItemIcon(ActionSlotInfo);
	}
	
	AddAbility(ActionSlotInfo);

	ActionSlots.Add(MoveTemp(ActionSlotInfo));
	
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
			RemoveAbility(*ActionSlotsTagDependency.Find(ActionSlots[i].ActionSlotIndex));
			ActionSlots[i].EntryType = EEntryType::None;
			ActionSlots[i].ItemIcon = nullptr;
			ActionSlots[i].ItemID = NAME_None;

			if (OnUpdateActionSlot.IsBound())
			{
				OnUpdateActionSlot.Broadcast(ActionSlots[i]);
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
			if (const FAbilityDefinition* ItemDef = GetAbilityData(ActionSlotInfo.ItemID))
			{
				ProjectN_AbilitySystemComponent->ServerAddAbility(ItemDef->Ability, *ActionSlotsTagDependency.Find(ActionSlotInfo.ActionSlotIndex));
				return;
			}
		case EEntryType::Item :
			if (const FItemDefinition* ItemDef = GetItemData(ActionSlotInfo.ItemID))
			{
				ProjectN_AbilitySystemComponent->ServerAddAbility(ItemDef->UseItemAbility, *ActionSlotsTagDependency.Find(ActionSlotInfo.ActionSlotIndex));
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

/*******************
*   Getters
********************/
const FAbilityDefinition* UProjectN_ActionBarController::GetAbilityData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindAbilityDefFromDataTable"));
	
	if (const FAbilityDefinition* AbilityDef = AbilityDataTable.LoadSynchronous()->FindRow<FAbilityDefinition>(ItemID, Context, false))
	{
		return AbilityDef;
	}
	return nullptr;
}

const FItemDefinition* UProjectN_ActionBarController::GetItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindItemFromDataTable"));
	
	if (const FItemDefinition* ItemDef = ItemsDataTable.LoadSynchronous()->FindRow<FItemDefinition>(ItemID, Context, false))
	{
		return ItemDef;
	}
	return nullptr;
}

const FEquippableItemDefinition* UProjectN_ActionBarController::GetEquippableItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindEquippableItemFromDataTable"));
	
	if (const FEquippableItemDefinition* WeaponItemDef = EquippableItemDataTable.LoadSynchronous()->FindRow<FEquippableItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FWeaponItemDefinition* UProjectN_ActionBarController::GetWeaponData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindWeaponFromDataTable"));
	
	if (const FWeaponItemDefinition* WeaponItemDef = WeaponItemsDataTable.LoadSynchronous()->FindRow<FWeaponItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FBagDefinition* UProjectN_ActionBarController::GetBagData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindBagFromDataTable"));
	
	if (const FBagDefinition* BagDef = BagDataTable.LoadSynchronous()->FindRow<FBagDefinition>(ItemID, Context, false))
	{
		return BagDef;
	}
	return nullptr;
}