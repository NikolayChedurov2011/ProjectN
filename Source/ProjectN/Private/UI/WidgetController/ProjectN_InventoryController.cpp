// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_InventoryController.h"

#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_InventoryComponent.h"

void UProjectN_InventoryController::BindCallbacksToResponce()
{
	InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
	{
		InventoryComponent->OnBagChanged.BindLambda([this](const FBagData& BagData)
		{
			if (OnUpdateBag.IsBound())
			{
				OnUpdateBag.Broadcast(BagData);
			}
		});

		InventoryComponent->OnEquipSlotChange.BindLambda([this](const FEquipSlotData& ItemData)
		{
			FEquipSlotData NewItemData = ItemData;
			
			if (NewItemData.ItemIcon == nullptr)
			{
				LoadItemIconFromStruct(NewItemData);
			}
			if (OnUpdateEquipSlot.IsBound())
			{
				OnUpdateEquipSlot.Broadcast(NewItemData);
			}
		});

		InventoryComponent->OnInventorySlotChange.BindLambda([this](const FInventorySlotData& ItemData)
		{
			FInventorySlotData NewItemData = ItemData;
			
			if (NewItemData.ItemIcon == nullptr)
			{
				LoadItemIconFromStruct(NewItemData);
			}
			if (OnUpdateInventorySlot.IsBound())
			{
				OnUpdateInventorySlot.Broadcast(NewItemData);
			}
		});
	}
}

void UProjectN_InventoryController::BroadcastInitialValues()
{
	InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
	{
		// TODO: Init inventory items
		for (int32 i = 0; i < InventoryComponent->GetBags().Num(); i++)
		{
			if (OnUpdateBag.IsBound())
			{
				OnUpdateBag.Broadcast(InventoryComponent->GetBags()[i]);
			}
		}
	}
}

void UProjectN_InventoryController::LoadItemIconFromStruct(FInventorySlotData& InventorySlotData) const
{
	LoadIcon(InventorySlotData.ItemID, InventorySlotData.ItemType, InventorySlotData.ItemIcon);
}

void UProjectN_InventoryController::LoadItemIconFromStruct(FEquipSlotData& EquippedSlotData) const
{
	LoadIcon(EquippedSlotData.ItemID, EquippedSlotData.ItemType, EquippedSlotData.ItemIcon);
}

void UProjectN_InventoryController::LoadIcon(const FName& ItemID, const EEntryType ItemType, UTexture2D*& ItemIcon) const
{
	switch (ItemType)
	{
	case EEntryType::Item :
		if (const FItemDefinition* ItemDef = GetItemData(ItemID))
		{
			ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::Equipment :
		if (const FEquippableItemDefinition* ItemDef = GetEquippableItemData(ItemID))
		{
			ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::Weapon :
		if (const FWeaponItemDefinition* ItemDef = GetWeaponData(ItemID))
		{
			ItemIcon = ItemDef->ItemIcon;
			return;
		}
	case EEntryType::Bag :
		if (const FBagDefinition* ItemDef = GetBagData(ItemID))
		{
			ItemIcon = ItemDef->ItemIcon;
			return;
		}
	default: ;
	}
}


/*********************************
 *  Items manage
 *********************************/
void UProjectN_InventoryController::TryAddItem(const FName ItemID, const EEntryType ItemType, const int32 Quantity) const
{
	InventoryComponent->TryAddItem(ItemID, ItemType, Quantity);
}

void UProjectN_InventoryController::RemoveItem(const int32 FromBagIndex, const int32 FromSlotIndex) const
{
	InventoryComponent->RemoveItem(FromBagIndex, FromSlotIndex);
}

void UProjectN_InventoryController::ReplaceItemsInBag(const int32 FromBagIndex, const int32 ToBagIndex,	const int32 FromSlotIndex, const int32 ToSlotIndex) const
{
	InventoryComponent->ReplaceItemInBag(FromBagIndex, FromSlotIndex, ToBagIndex, ToSlotIndex);
}



/*********************************
*  Equipping manage
*********************************/
void UProjectN_InventoryController::EquipItemToSlot(const FName ItemID, const EEntryType ItemType,	const EEquipSlot ToSlot) const
{
	InventoryComponent->EquipItemToSlot(ItemID, ItemType, ToSlot);
}

void UProjectN_InventoryController::UnEquipSlot(const EEquipSlot Slot) const
{
	InventoryComponent->UnEquipSlot(Slot);
}



/*******************
*   Getters
********************/
const FItemDefinition* UProjectN_InventoryController::GetItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindItemFromDataTable"));
	
	if (const FItemDefinition* ItemDef = ItemsDataTable.LoadSynchronous()->FindRow<FItemDefinition>(ItemID, Context, false))
	{
		return ItemDef;
	}
	return nullptr;
}

const FEquippableItemDefinition* UProjectN_InventoryController::GetEquippableItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindEquippableItemFromDataTable"));
	
	if (const FEquippableItemDefinition* WeaponItemDef = EquipmentItemsDataTable.LoadSynchronous()->FindRow<FEquippableItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FWeaponItemDefinition* UProjectN_InventoryController::GetWeaponData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindWeaponFromDataTable"));
	
	if (const FWeaponItemDefinition* WeaponItemDef = WeaponItemsDataTable.LoadSynchronous()->FindRow<FWeaponItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FBagDefinition* UProjectN_InventoryController::GetBagData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindBagFromDataTable"));
	
	if (const FBagDefinition* BagDef = BagDataTable.LoadSynchronous()->FindRow<FBagDefinition>(ItemID, Context, false))
	{
		return BagDef;
	}
	return nullptr;
}