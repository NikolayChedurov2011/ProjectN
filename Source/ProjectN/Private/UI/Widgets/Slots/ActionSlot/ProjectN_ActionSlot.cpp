// N Chedurov All Rights Reserved

#include "UI/Widgets/Slots/ActionSlot/ProjectN_ActionSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/ProjectN_DragDropOperation.h"
#include "UI/WidgetController/ProjectN_ActionBarController.h"
#include "UI/Widgets/Inventory/DragDropItem/ProjectN_DragDropItem.h"

void UProjectN_ActionSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CostText->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UProjectN_ActionSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && !ItemID.IsNone())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return FReply::Handled();
}

void UProjectN_ActionSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,	UDragDropOperation*& OutOperation)
{
	FActionSlotData ActionSlotDragPayload;
	ActionSlotDragPayload.ItemID = ItemID;
	ActionSlotDragPayload.ActionSlotIndex = SlotIndex;
		
	UProjectN_ActionSlotDragDropOperation* InventorySlotDragOperation = UProjectN_ActionSlotDragDropOperation::New(ActionSlotDragPayload);

	UProjectN_DragDropItem* DragDropItem = CreateWidget<UProjectN_DragDropItem>(GetOwningPlayer(), DragDropWidgetClass);
	
	DragDropItem->SetItemIcon(GetIconBrush());
	
	InventorySlotDragOperation->DefaultDragVisual = DragDropItem;
	InventorySlotDragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = InventorySlotDragOperation;

	OnSlotUnhovered.Broadcast();

	ClearSlot();
}

bool UProjectN_ActionSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,	UDragDropOperation* InOperation)
{
	if (UProjectN_InventorySlotDragDropOperation* InventorySlotDropOperation = Cast<UProjectN_InventorySlotDragDropOperation>(InOperation))
	{
		if (!InventorySlotDropOperation || InventorySlotDropOperation->Payload)
        {
        	return false;
        }
    
		Cast<UProjectN_ActionBarController>(WidgetController)->UpdateActionSlot(SlotIndex, InventorySlotDropOperation->MyPayload.ItemID);
        
        return true;
	}

	if (const UProjectN_EquipSlotDragDropOperation* EquipmentSlotDragOperation = Cast<UProjectN_EquipSlotDragDropOperation>(InOperation))
	{
		if (!EquipmentSlotDragOperation || EquipmentSlotDragOperation->Payload)
		{
			return false;
		}

		Cast<UProjectN_ActionBarController>(WidgetController)->UpdateActionSlot(SlotIndex, EquipmentSlotDragOperation->MyPayload.ItemID);
        
		return true;
	}

	if (const UProjectN_AbilitySlotDragDropOperation* AbilitySlotDragOperation = Cast<UProjectN_AbilitySlotDragDropOperation>(InOperation))
	{
		if (!AbilitySlotDragOperation || AbilitySlotDragOperation->Payload)
		{
			return false;
		}

		Cast<UProjectN_ActionBarController>(WidgetController)->UpdateActionSlot(SlotIndex, AbilitySlotDragOperation->MyPayload.ItemID);
        
		return true;
	}

	if (const UProjectN_ActionSlotDragDropOperation* ActionSlotDragOperation = Cast<UProjectN_ActionSlotDragDropOperation>(InOperation))
	{
		if (!ActionSlotDragOperation || ActionSlotDragOperation->Payload)
		{
			return false;
		}

		if (!ItemID.IsNone())
		{
			Cast<UProjectN_ActionBarController>(WidgetController)->ClearActionSlot(ActionSlotDragOperation->MyPayload.ActionSlotIndex);
			Cast<UProjectN_ActionBarController>(WidgetController)->UpdateActionSlot(ActionSlotDragOperation->MyPayload.ActionSlotIndex, ItemID);

			Cast<UProjectN_ActionBarController>(WidgetController)->ClearActionSlot(SlotIndex);
			Cast<UProjectN_ActionBarController>(WidgetController)->UpdateActionSlot(SlotIndex, ActionSlotDragOperation->MyPayload.ItemID);
		}
		else
		{
			Cast<UProjectN_ActionBarController>(WidgetController)->UpdateActionSlot(SlotIndex, ActionSlotDragOperation->MyPayload.ItemID);
		}
		
		return false;
	}

	return false;
}

void UProjectN_ActionSlot::SetItemIcon(const FSlateBrush& IconBrush) const
{
	ActionSlotImage->SetBrush(IconBrush);
}

void UProjectN_ActionSlot::SetCostText(const int32 CostValue) const
{
	const float AbsCostValue = FMath::Abs(CostValue);
	if (AbsCostValue > 0)
	{
		CostText->SetVisibility(ESlateVisibility::Visible);
		CostText->SetText(FText::AsNumber(AbsCostValue));
	}
	else
	{
		CostText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UProjectN_ActionSlot::SetCooldownText(const float NewCooldownValueRemaining) const
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

void UProjectN_ActionSlot::UpdateCooldownProgressBar(const float NewCooldownValueRemaining) const
{
	CooldownProgressBar->SetPercent(NewCooldownValueRemaining);
}

void UProjectN_ActionSlot::ClearSlot()
{
	SetItemIcon(FSlateNoResource());
	SetItemID(NAME_None);
	CooldownTag = FGameplayTag();
	BaseCooldownValue = 0.f;
	SetCooldownValueRemaining(0.f);
	SetCostText(0);
	UpdateCooldownProgressBar(CooldownValueRemaining);
}
