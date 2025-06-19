// N Chedurov All Rights Reserved


#include "UI/Widgets/Inventory/ProjectN_EquipmentWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Widgets/Description/ProjectN_EntryDescription.h"
#include "UI/Widgets/Slots/EquipSlot/ProjectN_EquipSlot.h"

void UProjectN_EquipmentWidget::OnSlotHovered(UProjectN_EntrySlotBase* HoveredWidget)
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

void UProjectN_EquipmentWidget::OnSlotUnhovered()
{
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
}

void UProjectN_EquipmentWidget::BindToEquipSlotsEvent()
{
	for (const TObjectPtr<UProjectN_EquipSlot> EquipmentSlot : EquipmentSlots)
	{
		EquipmentSlot->OnSlotHovered.AddDynamic(this, &UProjectN_EquipmentWidget::OnSlotHovered);
		EquipmentSlot->OnSlotUnhovered.AddDynamic(this, &UProjectN_EquipmentWidget::OnSlotUnhovered);
	}
}

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

UProjectN_EntryDescription* UProjectN_EquipmentWidget::GetItemDescription()
{
	if (!IsValid(DescriptionWidget))
	{
		DescriptionWidget = CreateWidget<UProjectN_EntryDescription>(GetOwningPlayer(), DescriptionWidgetClass);
		CanvasPanel->AddChild(DescriptionWidget);
	}
	return DescriptionWidget;
}

void UProjectN_EquipmentWidget::SetDescriptionSizeAndPosition(UProjectN_EntrySlotBase* HoveredWidget)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DescriptionWidget);
	const FVector2D DescriptionSize = DescriptionWidget->GetBoxSize();
	
	CanvasPanelSlot->SetSize(DescriptionSize);
	
	CanvasPanelSlot->SetPosition(GetClampedWidgetPosition(GetWidgetSize(CanvasPanel), DescriptionSize, HoveredWidget));
}

FVector2D UProjectN_EquipmentWidget::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, UProjectN_EntrySlotBase* HoveredWidget)
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

FVector2D UProjectN_EquipmentWidget::GetWidgetPosition(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();
	
	FVector2D PixelPos;
	FVector2D ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(UserWidget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry),PixelPos, ViewportPos);
	
	return ViewportPos;
}

FVector2D UProjectN_EquipmentWidget::GetWidgetSize(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();

	return Geometry.GetLocalSize();
}