// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_InventoryController.h"

#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_InventoryComponent.h"

void UProjectN_InventoryController::BindCallbacksToResponce()
{
	UProjectN_InventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
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
	}
}

void UProjectN_InventoryController::BroadcastInitialValues()
{
	UProjectN_InventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
	{
		

		// TODO: Init inventory items
		for (int32 i = 0; i < InventoryComponent->GetBags().Num(); i++)
		{
			if (OnUpdateBag.IsBound())
			{
				OnUpdateBag.Broadcast(i, InventoryComponent->GetBags()[i].Slots.Num());
			}
		}
	}
}

