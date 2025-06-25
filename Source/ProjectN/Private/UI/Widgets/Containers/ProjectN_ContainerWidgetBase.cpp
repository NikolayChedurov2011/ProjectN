// N Chedurov All Rights Reserved


#include "UI/Widgets/Containers/ProjectN_ContainerWidgetBase.h"

#include "AbilitySystem/ProjectN_AbilitySystemLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"
#include "UI/Widgets/Description/ProjectN_EntryDescription.h"
#include "UI/Widgets/Slots/ProjectN_EntrySlotBase.h"

void UProjectN_ContainerWidgetBase::OnSlotHovered(UProjectN_EntrySlotBase* HoveredWidget)
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetItemDescription()->Collapse();

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, HoveredWidget]()
	{
		UItemManifest* Manifest = UProjectN_AbilitySystemLibrary::GetOverlayWidgetController(this)->FindManifest(HoveredWidget->GetEntryID());
		Manifest->AssimilateInventoryFragments(GetItemDescription());
		
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		SetDescriptionSizeAndPosition(HoveredWidget);
	});
	
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimerHandle, TimerDelegate, DescriptionTimerShowAt, false);
}

void UProjectN_ContainerWidgetBase::OnSlotUnhovered()
{
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimerHandle);
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetItemDescription()->Collapse();
}

UProjectN_EntryDescription* UProjectN_ContainerWidgetBase::GetItemDescription()
{
	if (!IsValid(DescriptionWidget))
	{
		DescriptionWidget = CreateWidget<UProjectN_EntryDescription>(GetOwningPlayer(), DescriptionWidgetClass);
		CanvasPanel->AddChild(DescriptionWidget);
	}
	return DescriptionWidget;
}

void UProjectN_ContainerWidgetBase::SetDescriptionSizeAndPosition(UProjectN_EntrySlotBase* HoveredWidget)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DescriptionWidget);
	const FVector2D DescriptionSize = DescriptionWidget->GetBoxSize();
	
	CanvasPanelSlot->SetSize(DescriptionSize);
	
	CanvasPanelSlot->SetPosition(GetClampedWidgetPosition(GetWidgetSize(CanvasPanel), DescriptionSize, HoveredWidget));
}

FVector2D UProjectN_ContainerWidgetBase::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, UProjectN_EntrySlotBase* HoveredWidget)
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

FVector2D UProjectN_ContainerWidgetBase::GetWidgetPosition(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();
	
	FVector2D PixelPos;
	FVector2D ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(UserWidget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry),PixelPos, ViewportPos);
	
	return ViewportPos;
}

FVector2D UProjectN_ContainerWidgetBase::GetWidgetSize(UWidget* UserWidget)
{
	const FGeometry Geometry = UserWidget->GetCachedGeometry();

	return Geometry.GetLocalSize();
}