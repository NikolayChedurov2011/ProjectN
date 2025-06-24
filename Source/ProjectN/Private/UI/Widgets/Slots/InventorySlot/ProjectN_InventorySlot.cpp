// N Chedurov All Rights Reserved


#include "UI/Widgets/Slots/InventorySlot/ProjectN_InventorySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/ProjectN_DragDropOperation.h"
#include "UI/WidgetController/ProjectN_InventoryController.h"
#include "UI/Widgets/DragDropItem/ProjectN_DragDropItem.h"

void UProjectN_InventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StackCount->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UProjectN_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && !ItemID.IsNone())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return FReply::Handled();
}

void UProjectN_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	FInventorySlotData ItemDragPayload;
	ItemDragPayload.BagIndex = BagIndex;
	ItemDragPayload.SlotIndex = BagSlotIndex;
	ItemDragPayload.ItemID = ItemID;
	ItemDragPayload.Quantity = Stack;
		
	UProjectN_InventorySlotDragDropOperation* InventorySlotDragOperation = UProjectN_InventorySlotDragDropOperation::New(ItemDragPayload);

	UProjectN_DragDropItem* DragDropItem = CreateWidget<UProjectN_DragDropItem>(GetOwningPlayer(), DragDropWidgetClass);
	
	DragDropItem->SetItemIcon(GetIconBrush());
	DragDropItem->SetStackCount(Stack);
	
	InventorySlotDragOperation->DefaultDragVisual = DragDropItem;
	InventorySlotDragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = InventorySlotDragOperation;

	OnSlotUnhovered.Broadcast();
}

bool UProjectN_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UProjectN_InventorySlotDragDropOperation* InventorySlotDropOperation = Cast<UProjectN_InventorySlotDragDropOperation>(InOperation))
	{
		if (!InventorySlotDropOperation || InventorySlotDropOperation->Payload)
        	{
        		return false;
        	}
        
        	if (!ItemID.IsNone() && ItemID == InventorySlotDropOperation->MyPayload.ItemID)
        	{
        		Cast<UProjectN_InventoryController>(WidgetController)->StackItems(ItemID, InventorySlotDropOperation->MyPayload.BagIndex, BagIndex, InventorySlotDropOperation->MyPayload.SlotIndex, BagSlotIndex, InventorySlotDropOperation->MyPayload.Quantity);
        	}
        	else if ((BagIndex != InventorySlotDropOperation->MyPayload.BagIndex || BagSlotIndex != InventorySlotDropOperation->MyPayload.SlotIndex) && !ItemID.IsNone())
        	{
        		Cast<UProjectN_InventoryController>(WidgetController)->RemoveItem(InventorySlotDropOperation->MyPayload.BagIndex, InventorySlotDropOperation->MyPayload.SlotIndex);
        		Cast<UProjectN_InventoryController>(WidgetController)->TryAddItemToSlot(InventorySlotDropOperation->MyPayload.BagIndex, InventorySlotDropOperation->MyPayload.SlotIndex, ItemID, Stack);

        		Cast<UProjectN_InventoryController>(WidgetController)->RemoveItem(BagIndex, BagSlotIndex);
        		Cast<UProjectN_InventoryController>(WidgetController)->TryAddItemToSlot(BagIndex, BagSlotIndex, InventorySlotDropOperation->MyPayload.ItemID, InventorySlotDropOperation->MyPayload.Quantity);
        	}
        	else
        	{
        		Cast<UProjectN_InventoryController>(WidgetController)->RemoveItem(InventorySlotDropOperation->MyPayload.BagIndex, InventorySlotDropOperation->MyPayload.SlotIndex);
        		Cast<UProjectN_InventoryController>(WidgetController)->TryAddItemToSlot(BagIndex, BagSlotIndex, InventorySlotDropOperation->MyPayload.ItemID, InventorySlotDropOperation->MyPayload.Quantity);
        	}
        	
        	return true;
	}

	if (const UProjectN_EquipSlotDragDropOperation* EquipmentSlotDragOperation = Cast<UProjectN_EquipSlotDragDropOperation>(InOperation))
	{
		if (!EquipmentSlotDragOperation || EquipmentSlotDragOperation->Payload)
		{
			return false;
		}
		
		if (!ItemID.IsNone())
		{
			const float TempStack = Stack;
			const FName TempID = ItemID;
			Cast<UProjectN_InventoryController>(WidgetController)->RemoveItem(BagIndex, BagSlotIndex);
			Cast<UProjectN_InventoryController>(WidgetController)->TryAddItemToSlot(BagIndex, BagSlotIndex, EquipmentSlotDragOperation->MyPayload.ItemID, 1);
			Cast<UProjectN_InventoryController>(WidgetController)->EquipItemToSlot(TempID, EquipmentSlotDragOperation->MyPayload.EquipSlot, TempStack);

			return true;
		}

		Cast<UProjectN_InventoryController>(WidgetController)->TryAddItemToSlot(BagIndex, BagSlotIndex, EquipmentSlotDragOperation->MyPayload.ItemID, 1);
	
		return true;
	}

	return false;
}

void UProjectN_InventorySlot::SetItemIcon(const FSlateBrush& IconBrush) const
{
	ItemIcon->SetBrush(IconBrush);
}

void UProjectN_InventorySlot::SetStackCountText(const int32 NewStackCount) const
{
	if (NewStackCount > 1)
	{
		StackCount->SetVisibility(ESlateVisibility::Visible);
		StackCount->SetText(FText::AsNumber(NewStackCount));
	}
	else
	{
		StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UProjectN_InventorySlot::SetCooldownText(const float NewCooldownValueRemaining) const
{
	if (NewCooldownValueRemaining > 0)
	{
		FNumberFormattingOptions Opt;
		Opt.MaximumFractionalDigits = 1;
		CooldownText->SetVisibility(ESlateVisibility::Visible);
		CooldownText->SetText(FText::AsNumber(NewCooldownValueRemaining, &Opt));
	}
	else
	{
		CooldownText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UProjectN_InventorySlot::UpdateCooldownProgressBar(const float NewCooldownValueRemaining) const
{
	CooldownProgressBar->SetPercent(NewCooldownValueRemaining);
}

void UProjectN_InventorySlot::ClearSlot()
{
	SetItemIcon(FSlateNoResource());
	SetStackCount(0);
	SetItemID(NAME_None);
	CooldownTag = FGameplayTag();
	BaseCooldownValue = 0.f;
	SetCooldownValueRemaining(0.f);
	UpdateCooldownProgressBar(CooldownValueRemaining);
}