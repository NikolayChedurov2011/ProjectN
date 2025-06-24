// N Chedurov All Rights Reserved


#include "UI/Widgets/Containers/ProjectN_InventoryWidget.h"

#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "UI/Widgets/Slots/InventorySlot/ProjectN_InventorySlot.h"

UProjectN_InventorySlot* UProjectN_InventoryWidget::FindInventorySlot(const FGuid BagIndex, const int32 BagSlotIndex)
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

void UProjectN_InventoryWidget::InitNewBag(const FGuid BagIndex, const int32 SlotIndex)
{
	if (IsBagExist(BagIndex))
	{
		return;
	}
	
	for (int32 i = 0; i < SlotIndex; i++)
	{
		UProjectN_InventorySlot* NewSlot = CreateWidget<UProjectN_InventorySlot>(GetOwningPlayer(), InventorySlotClass);
		NewSlot->SetBagIndex(BagIndex);
		NewSlot->SetBagSlotIndex(i);
		NewSlot->SetWidgetController(WidgetController);
		
		InventoryGrid->AddChild(NewSlot);
		InventorySlots.Add(NewSlot);

		NewSlot->OnSlotHovered.AddDynamic(this, &UProjectN_InventoryWidget::OnSlotHovered);
		NewSlot->OnSlotUnhovered.AddDynamic(this, &UProjectN_InventoryWidget::OnSlotUnhovered);
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

bool UProjectN_InventoryWidget::IsBagExist(const FGuid BagIndex)
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

void UProjectN_InventoryWidget::ClearInventorySlot(const FGuid BagIndex, const int32 BagSlotIndex)
{
	FindInventorySlot(BagIndex, BagSlotIndex)->ClearSlot();
}