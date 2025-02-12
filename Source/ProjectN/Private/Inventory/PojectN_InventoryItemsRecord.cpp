// N Chedurov All Rights Reserved


#include "Inventory/PojectN_InventoryItemsRecord.h"

#include "ProjectN/ProjectNTypes.h"

void FInventoryList::AddItem(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass)
{
	FInventoryItem& Item = Items.AddDefaulted_GetRef();

	Item.ItemInstance = NewObject<UProjectN_ItemInstance>();
	Item.ItemInstance->Init(ItemStaticDataClass);

	MarkItemDirty(Item);
}

void FInventoryList::RemoveItem(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->IsA(ItemStaticDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}
