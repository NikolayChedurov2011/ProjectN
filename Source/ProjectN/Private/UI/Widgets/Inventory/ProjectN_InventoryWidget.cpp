// N Chedurov All Rights Reserved


#include "UI/Widgets/Inventory/ProjectN_InventoryWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/Widgets/Description/ProjectN_EntryDescription.h"
#include "UI/Widgets/Slots/InventorySlot/ProjectN_InventorySlot.h"

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

		NewSlot->OnSlotHovered.AddDynamic(this, &UProjectN_InventoryWidget::OnSlotHovered);
		NewSlot->OnSlotUnhovered.AddDynamic(this, &UProjectN_InventoryWidget::OnSlotUnhovered);
	}
}

void UProjectN_InventoryWidget::OnSlotHovered(UProjectN_EntrySlotBase* HoveredWidget)
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, HoveredWidget]()
	{
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		SetDescriptionSizeAndPosition(HoveredWidget);
	});
	
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimerHandle, TimerDelegate, DescriptionTimerShowAt, false);
}

void UProjectN_InventoryWidget::OnSlotUnhovered()
{
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
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

//////////
UProjectN_EntryDescription* UProjectN_InventoryWidget::GetItemDescription()
{
	if (!IsValid(DescriptionWidget))
	{
		DescriptionWidget = CreateWidget<UProjectN_EntryDescription>(GetOwningPlayer(), DescriptionWidgetClass);
		CanvasPanel->AddChild(DescriptionWidget);
	}
	return DescriptionWidget;
}

void UProjectN_InventoryWidget::SetDescriptionSizeAndPosition(UProjectN_EntrySlotBase* HoveredWidget)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DescriptionWidget);
	const FVector2D DescriptionSize = DescriptionWidget->GetBoxSize();
	
	CanvasPanelSlot->SetSize(DescriptionSize);
	
	CanvasPanelSlot->SetPosition(GetClampedWidgetPosition(GetWidgetSize(CanvasPanel), DescriptionSize, HoveredWidget));
}

FVector2D UProjectN_InventoryWidget::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, UProjectN_EntrySlotBase* HoveredWidget)
{
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	MousePos.Y -= WidgetSize.Y;
	FVector2D ClampedPos = MousePos;

	if (ClampedPos.X + WidgetSize.X > Boundary.X)
	{
		ClampedPos.X = Boundary.X - WidgetSize.X;
	}
	if (ClampedPos.X < 0.f)
	{
		ClampedPos.X = 0.f;
	}

	return ClampedPos;
}

FVector2D UProjectN_InventoryWidget::GetWidgetPosition(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();
	
	FVector2D PixelPos;
	FVector2D ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(UserWidget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry),PixelPos, ViewportPos);
	
	return ViewportPos;
}

FVector2D UProjectN_InventoryWidget::GetWidgetSize(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();

	return Geometry.GetLocalSize();
}