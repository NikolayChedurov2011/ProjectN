// N Chedurov All Rights Reserved

#include "UI/Widgets/Inventory/ProjectN_AbilityBookWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "UI/Widgets/Description/ProjectN_EntryDescription.h"
#include "UI/Widgets/Slots/ProjectN_EntrySlotBase.h"
#include "UI/Widgets/Slots/AbilityBookSlot/ProjectN_AbilityBookSlot.h"

UProjectN_AbilityBookSlot* UProjectN_AbilityBookWidget::FindAbility(const FName& AbilityID)
{
	for (const TObjectPtr<UProjectN_AbilityBookSlot>& Ability : AbilitySlots)
	{
		if (Ability->GetItemID() == AbilityID)
		{
			return Ability;
		}
	}

	return nullptr;
}

UProjectN_AbilityBookSlot* UProjectN_AbilityBookWidget::AddNewAbility()
{
	UProjectN_AbilityBookSlot* NewAbility = CreateWidget<UProjectN_AbilityBookSlot>(GetOwningPlayer(), AbilitySlotClass);
	
	AbilitySlots.Add(NewAbility);
	AbilityBookGrid->AddChildToGrid(NewAbility, (AbilitySlots.Num() - 1) / Columns, (AbilitySlots.Num() - 1) % Columns);
	
	NewAbility->OnSlotHovered.AddDynamic(this, &UProjectN_AbilityBookWidget::OnSlotHovered);
	NewAbility->OnSlotUnhovered.AddDynamic(this, &UProjectN_AbilityBookWidget::OnSlotUnhovered);
	return NewAbility;
}

void UProjectN_AbilityBookWidget::OnSlotHovered(UProjectN_EntrySlotBase* HoveredWidget)
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

void UProjectN_AbilityBookWidget::OnSlotUnhovered()
{
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
}


UProjectN_EntryDescription* UProjectN_AbilityBookWidget::GetItemDescription()
{
	if (!IsValid(DescriptionWidget))
	{
		DescriptionWidget = CreateWidget<UProjectN_EntryDescription>(GetOwningPlayer(), DescriptionWidgetClass);
		CanvasPanel->AddChild(DescriptionWidget);
	}
	return DescriptionWidget;
}

void UProjectN_AbilityBookWidget::SetDescriptionSizeAndPosition(UProjectN_EntrySlotBase* HoveredWidget)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DescriptionWidget);
	const FVector2D DescriptionSize = DescriptionWidget->GetBoxSize();
	
	CanvasPanelSlot->SetSize(DescriptionSize);
	
	CanvasPanelSlot->SetPosition(GetClampedWidgetPosition(GetWidgetSize(CanvasPanel), DescriptionSize, HoveredWidget));
}

FVector2D UProjectN_AbilityBookWidget::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, UProjectN_EntrySlotBase* HoveredWidget)
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

FVector2D UProjectN_AbilityBookWidget::GetWidgetPosition(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();
	
	FVector2D PixelPos;
	FVector2D ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(UserWidget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry),PixelPos, ViewportPos);
	
	return ViewportPos;
}

FVector2D UProjectN_AbilityBookWidget::GetWidgetSize(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();

	return Geometry.GetLocalSize();
}