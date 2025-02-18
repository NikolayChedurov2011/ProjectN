// N Chedurov All Rights Reserved


#include "Inventory/PojectN_InventoryItemsRecord.h"

#include "ProjectN/ProjectNTypes.h"

void FInventoryList::AddItemByStaticClass(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass)
{
	FInventoryItem& Item = Items.AddDefaulted_GetRef();

	Item.ItemInstance = NewObject<UProjectN_ItemInstance>();
	Item.ItemInstance->Init(ItemStaticDataClass);

	MarkItemDirty(Item);
}

void FInventoryList::AddItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	FInventoryItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;

	MarkItemDirty(Item);
}

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
