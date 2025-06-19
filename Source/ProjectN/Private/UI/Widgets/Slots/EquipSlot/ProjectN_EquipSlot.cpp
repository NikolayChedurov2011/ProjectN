// N Chedurov All Rights Reserved


#include "UI/Widgets/Slots/EquipSlot/ProjectN_EquipSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/ProjectN_DragDropOperation.h"
#include "UI/WidgetController/ProjectN_InventoryController.h"
#include "UI/Widgets/Inventory/DragDropItem/ProjectN_DragDropItem.h"

void UProjectN_EquipSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (IsValid(DefaultSlotIcon))
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(DefaultSlotIcon);
		SetItemIcon(NewBrush);
	}
}

FReply UProjectN_EquipSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && !ItemID.IsNone())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return FReply::Handled();
}

void UProjectN_EquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	FEquipSlotData EquipDragPayload;
	EquipDragPayload.ItemID = ItemID;
	EquipDragPayload.EquipSlot = EquipSlot;
		
	UProjectN_EquipSlotDragDropOperation* EquipmentSlotDragOperation = UProjectN_EquipSlotDragDropOperation::New(EquipDragPayload);

	UProjectN_DragDropItem* DragDropItem = CreateWidget<UProjectN_DragDropItem>(GetOwningPlayer(), DragDropWidgetClass);
	
	DragDropItem->SetItemIcon(GetIconBrush());
	
	EquipmentSlotDragOperation->DefaultDragVisual = DragDropItem;
	EquipmentSlotDragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = EquipmentSlotDragOperation;

	Cast<UProjectN_InventoryController>(WidgetController)->UnEquipSlot(EquipSlot);
}

bool UProjectN_EquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (const UProjectN_EquipSlotDragDropOperation* EquipmentSlotDragOperation = Cast<UProjectN_EquipSlotDragDropOperation>(InOperation))
	{
		if (!EquipmentSlotDragOperation || EquipmentSlotDragOperation->Payload)
		{
			return false;
		}

		Cast<UProjectN_InventoryController>(WidgetController)->EquipItemToSlot(EquipmentSlotDragOperation->MyPayload.ItemID, EquipSlot);
	
		return true;
	}

	if (const UProjectN_InventorySlotDragDropOperation* InventorySlotDragOperation = Cast<UProjectN_InventorySlotDragDropOperation>(InOperation))
	{
		if (!InventorySlotDragOperation || InventorySlotDragOperation->Payload)
		{
			return false;
		}

		Cast<UProjectN_InventoryController>(WidgetController)->RemoveItem(InventorySlotDragOperation->MyPayload.BagIndex, InventorySlotDragOperation->MyPayload.SlotIndex);
		Cast<UProjectN_InventoryController>(WidgetController)->EquipItemToSlot(InventorySlotDragOperation->MyPayload.ItemID, EquipSlot, InventorySlotDragOperation->MyPayload.Quantity);
	
		return true;
	}

	return false;
}

void UProjectN_EquipSlot::SetItemIcon(const FSlateBrush& IconBrush) const
{
	ItemIcon->SetBrush(IconBrush);
}

void UProjectN_EquipSlot::ClearSlot()
{
	if (IsValid(DefaultSlotIcon))
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(DefaultSlotIcon);
		SetItemIcon(NewBrush);
	}
	SetItemID(NAME_None);
}
