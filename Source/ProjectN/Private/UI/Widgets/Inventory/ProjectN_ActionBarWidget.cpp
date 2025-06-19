// N Chedurov All Rights Reserved


#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Widgets/Description/ProjectN_EntryDescription.h"
#include "UI/Widgets/Inventory/ProjectN_ActionBartWidget.h"
#include "UI/Widgets/Slots/ActionSlot/ProjectN_ActionSlot.h"

void UProjectN_ActionBartWidget::OnSlotHovered(UProjectN_EntrySlotBase* HoveredWidget)
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

void UProjectN_ActionBartWidget::OnSlotUnhovered()
{
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
}

void UProjectN_ActionBartWidget::BindToActionSlotsEvent()
{
	for (const TObjectPtr<UProjectN_ActionSlot> ActionSlot : ActionSlots)
	{
		ActionSlot->OnSlotHovered.AddDynamic(this, &UProjectN_ActionBartWidget::OnSlotHovered);
		ActionSlot->OnSlotUnhovered.AddDynamic(this, &UProjectN_ActionBartWidget::OnSlotUnhovered);
	}
}

void UProjectN_ActionBartWidget::ClearActionSlot(const int32 SlotIndex)
{
	ActionSlot(SlotIndex)->ClearSlot();
}

void UProjectN_ActionBartWidget::ClearActionSlot(const FGameplayTag& InputTag)
{
	ActionSlot(InputTag)->ClearSlot();
}

UProjectN_ActionSlot* UProjectN_ActionBartWidget::ActionSlot(const int32 SlotIndex)
{
	for (const TObjectPtr<UProjectN_ActionSlot>& ActionSlot : ActionSlots)
	{
		if (ActionSlot->GetSlotIndex() == SlotIndex)
		{
			return ActionSlot;
		}
	}
	return nullptr;
}

UProjectN_ActionSlot* UProjectN_ActionBartWidget::ActionSlot(const FGameplayTag& InputTag)
{
	for (const TObjectPtr<UProjectN_ActionSlot>& ActionSlot : ActionSlots)
	{
		if (ActionSlot->GetInputTag() == InputTag)
		{
			return ActionSlot;
		}
	}
	return nullptr;
}

UProjectN_EntryDescription* UProjectN_ActionBartWidget::GetItemDescription()
{
	if (!IsValid(DescriptionWidget))
	{
		DescriptionWidget = CreateWidget<UProjectN_EntryDescription>(GetOwningPlayer(), DescriptionWidgetClass);
		CanvasPanel->AddChild(DescriptionWidget);
	}
	return DescriptionWidget;
}

void UProjectN_ActionBartWidget::SetDescriptionSizeAndPosition(UProjectN_EntrySlotBase* HoveredWidget)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DescriptionWidget);
	const FVector2D DescriptionSize = DescriptionWidget->GetBoxSize();
	
	CanvasPanelSlot->SetSize(DescriptionSize);
	
	CanvasPanelSlot->SetPosition(GetClampedWidgetPosition(GetWidgetSize(CanvasPanel), DescriptionSize, HoveredWidget));
}

FVector2D UProjectN_ActionBartWidget::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, UProjectN_EntrySlotBase* HoveredWidget)
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

FVector2D UProjectN_ActionBartWidget::GetWidgetPosition(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();
	
	FVector2D PixelPos;
	FVector2D ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(UserWidget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry),PixelPos, ViewportPos);
	
	return ViewportPos;
}

FVector2D UProjectN_ActionBartWidget::GetWidgetSize(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();

	return Geometry.GetLocalSize();
}