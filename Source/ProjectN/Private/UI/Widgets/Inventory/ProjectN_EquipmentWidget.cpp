// N Chedurov All Rights Reserved


#include "UI/Widgets/Inventory/ProjectN_EquipmentWidget.h"
#include "UI/Widgets/Slots/EquipSlot/ProjectN_EquipSlot.h"

void UProjectN_EquipmentWidget::ClearEquipmentSlot(const EEquipSlot SlotType)
{
	FindEquipmentSlot(SlotType)->ClearSlot();
}

UProjectN_EquipSlot* UProjectN_EquipmentWidget::FindEquipmentSlot(const EEquipSlot SlotType)
{
	if (SlotType == EEquipSlot::None)
	{
		return nullptr;
	}

	for (const TObjectPtr<UProjectN_EquipSlot>& EquipmentSlot : EquipmentSlots)
	{
		if (EquipmentSlot->GetEquipSlot() == SlotType)
		{
			return EquipmentSlot;
		}
	}
	return nullptr;
}
