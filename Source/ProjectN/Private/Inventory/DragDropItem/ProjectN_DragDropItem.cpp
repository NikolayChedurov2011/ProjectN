#include "UI/Widgets/Inventory/DragDropItem/ProjectN_DragDropItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UProjectN_DragDropItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StackCount->SetVisibility(ESlateVisibility::Collapsed);
}

void UProjectN_DragDropItem::SetItemIcon(const FSlateBrush& IconBrush) const
{
	ItemIcon->SetBrush(IconBrush);
}

void UProjectN_DragDropItem::SetStackCount(const int32 NewStackCount) const
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
