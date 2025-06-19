// N Chedurov All Rights Reserved


#include "UI/Widgets/Slots/AbilityBookSlot/ProjectN_AbilityBookSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/ProjectN_DragDropOperation.h"
#include "UI/WidgetController/ProjectN_InventoryController.h"
#include "UI/Widgets/Inventory/DragDropItem/ProjectN_DragDropItem.h"

FReply UProjectN_AbilityBookSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && !ItemID.IsNone())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return FReply::Handled();
}

void UProjectN_AbilityBookSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	FAbilitySlotData AbilityDragPayload;
	AbilityDragPayload.ItemID = ItemID;
		
	UProjectN_AbilitySlotDragDropOperation* AbilitySlotDragOperation = UProjectN_AbilitySlotDragDropOperation::New(AbilityDragPayload);

	UProjectN_DragDropItem* DragDropItem = CreateWidget<UProjectN_DragDropItem>(GetOwningPlayer(), DragDropWidgetClass);
	
	DragDropItem->SetItemIcon(GetIconBrush());
	
	AbilitySlotDragOperation->DefaultDragVisual = DragDropItem;
	AbilitySlotDragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = AbilitySlotDragOperation;
}

void UProjectN_AbilityBookSlot::SetAbilityIcon(const FSlateBrush& IconBrush) const
{
	AbilityIcon->SetBrush(IconBrush);
}
