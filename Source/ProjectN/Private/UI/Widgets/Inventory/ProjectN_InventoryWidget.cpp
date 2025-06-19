// N Chedurov All Rights Reserved


#include "UI/Widgets/Inventory/ProjectN_InventoryWidget.h"

#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/Widgets/Slots/InventorySlot/ProjectN_InventorySlot.h"
#include "UI/Widgets/Inventory/DragDropItem/ProjectN_DragDropItem.h"

UProjectN_InventorySlot* UProjectN_InventoryWidget::FindInventorySlot(const int32 BagIndex, const int32 BagSlotIndex)
{
	for (const TObjectPtr<UProjectN_InventorySlot>& InventorySlot : InventorySlots)
	{
		if (InventorySlot->GetBagIndex() == BagIndex && InventorySlot->GetBagSlotIndex() == BagSlotIndex)
		{
			return InventorySlot;
		}
	}

	return nullptr;
}

void UProjectN_InventoryWidget::InitNewBag(const FBagData& NewBagData)
{
	if (IsBagExist(NewBagData.BagIndex))
	{
		return;
	}
	
	for (int32 i = 0; i < NewBagData.Slots.Num(); i++)
	{
		UProjectN_InventorySlot* NewSlot = CreateWidget<UProjectN_InventorySlot>(GetOwningPlayer(), InventorySlotClass);
		NewSlot->SetBagIndex(NewBagData.BagIndex);
		NewSlot->SetBagSlotIndex(NewBagData.Slots[i].SlotIndex);
		NewSlot->SetItemID(NewBagData.Slots[i].ItemID);
		NewSlot->SetStackCount(NewBagData.Slots[i].Quantity);
		NewSlot->SetWidgetController(WidgetController);
		
		InventoryGrid->AddChild(NewSlot);
		InventorySlots.Add(NewSlot);
	}
}

bool UProjectN_InventoryWidget::IsRightMouseClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UProjectN_InventoryWidget::IsLeftMouseClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UProjectN_InventoryWidget::RemoveBag(const FBagData& NewBagData)
{
	for (const FInventorySlotData& SlotToRemove : NewBagData.Slots)
	{
		for (int32 i = 0; i < InventorySlots.Num(); i++)
		{
			if (InventorySlots[i]->GetBagIndex() == NewBagData.BagIndex && InventorySlots[i]->GetBagSlotIndex() == SlotToRemove.SlotIndex)
			{
				InventoryGrid->RemoveChild(InventorySlots[i]);
				InventorySlots.RemoveAt(i);
			}
		}
	}
}

bool UProjectN_InventoryWidget::IsBagExist(const int32 BagIndex)
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i]->GetBagIndex() == BagIndex)
		{
			return true;
		}
	}

	return false;
}

void UProjectN_InventoryWidget::ClearInventorySlot(const int32 BagIndex, const int32 BagSlotIndex)
{
	FindInventorySlot(BagIndex, BagSlotIndex)->ClearSlot();
}
