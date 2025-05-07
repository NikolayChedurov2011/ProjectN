// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_InventoryController.h"

#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_InventoryComponent.h"

void UProjectN_InventoryController::BindCallbacksToResponce()
{
	UProjectN_InventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
	{
		InventoryComponent->OnUpdateItem.BindLambda([this, InventoryComponent](UProjectN_ItemInstance* NewItemInstance)
		{
			FInventoryItemInfo Item;
			Item.ItemIcon = NewItemInstance->GetItemStaticClass()->GetItemIcon();
			Item.ItemName = NewItemInstance->GetItemStaticClass()->GetItemName().ToString();
			Item.ItemDescription = NewItemInstance->GetItemStaticClass()->GetItemDescription();
			Item.ItemStack = NewItemInstance->GetItemStack();
			Item.bCanBeEquipped = InventoryComponent->IsEquippableItem(NewItemInstance);
			Item.ItemInstance = NewItemInstance;

			OnInventoryItemAdded.Broadcast(Item);
		});
		
		InventoryComponent->OnRemoveItem.BindLambda([this](UProjectN_ItemInstance* ItemInstanceToRemove)
		{
			OnInventoryItemRemoved.Broadcast(ItemInstanceToRemove);
		});
	}
}

void UProjectN_InventoryController::BroadcastInitialValues()
{
	UProjectN_InventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
	{
		TArray<FInventoryItemInfo> ItemsInfo;
		for (const FInventoryItem ItemInfo : InventoryComponent->GetItemsList())
		{
			FInventoryItemInfo Item;
			Item.ItemIcon = ItemInfo.ItemInstance->GetItemStaticClass()->GetItemIcon();
			Item.ItemName = ItemInfo.ItemInstance->GetItemStaticClass()->GetItemName().ToString();
			Item.ItemDescription = ItemInfo.ItemInstance->GetItemStaticClass()->GetItemDescription();
			Item.ItemStack = ItemInfo.ItemInstance->GetItemStack();
			Item.bCanBeEquipped = InventoryComponent->IsEquippableItem(ItemInfo.ItemInstance);
			Item.ItemInstance = ItemInfo.ItemInstance;

			ItemsInfo.Add(Item);
		}

		OnInventoryItems.Broadcast(ItemsInfo);
	}
}

