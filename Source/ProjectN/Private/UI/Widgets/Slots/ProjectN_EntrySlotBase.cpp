// N Chedurov All Rights Reserved


#include "UI/Widgets/Slots/ProjectN_EntrySlotBase.h"

void UProjectN_EntrySlotBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (OnSlotHovered.IsBound() && !ItemID.IsNone())
	{
		OnSlotHovered.Broadcast(this);
	}
}

void UProjectN_EntrySlotBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (OnSlotUnhovered.IsBound() && !ItemID.IsNone())
	{
		OnSlotUnhovered.Broadcast();
	}
}