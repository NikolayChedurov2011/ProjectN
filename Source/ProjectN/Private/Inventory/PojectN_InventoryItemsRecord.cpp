// N Chedurov All Rights Reserved


#include "Inventory/PojectN_InventoryItemsRecord.h"

#include "ProjectN/ProjectNTypes.h"

void FInventoryList::AddItemByStaticClass(UObject* Outer, const TSubclassOf<UItemStaticClass>& ItemStaticDataClass, const int32 ItemStack)
{
	FInventoryItem& Item = Items.AddDefaulted_GetRef();
	
	const UItemStaticClass* StaticData = ItemStaticDataClass.GetDefaultObject();
	
	Item.ItemInstance = StaticData->CreateInstance(Outer);
	Item.ItemInstance->Init(ItemStaticDataClass);
	Item.ItemInstance->InitItemStack(ItemStack);

	MarkItemDirty(Item);
}

/*void FInventoryList::AddItemByInstance(UProjectN_ItemInstance* InItemInstance, const int32 ItemStack)
{
	FInventoryItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	Item.ItemInstance->InitItemStack(ItemStack);

	MarkItemDirty(Item);
}*/

void FInventoryList::RemoveItemByStaticClass(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticClass()->IsA(ItemStaticDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

void FInventoryList::RemoveItemByInstance(const UProjectN_ItemInstance* InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

FInventoryItem* FInventoryList::FindItemByInstance(const UProjectN_ItemInstance* InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			return &Item;
		}
	}
	return nullptr;
}

FInventoryItem* FInventoryList::FindItemByClassWithInstance(const UProjectN_ItemInstance* InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticClass() == InItemInstance->GetItemStaticClass())
		{
			return &Item;
		}
	}
	return nullptr;
}

FInventoryItem* FInventoryList::FindItemByClass(const UItemStaticClass* InItemClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticClass() == InItemClass)
		{
			return &Item;
		}
	}
	return nullptr;
}