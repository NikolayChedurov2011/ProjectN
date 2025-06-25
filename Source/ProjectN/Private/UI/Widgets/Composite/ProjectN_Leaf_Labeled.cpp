// N Chedurov All Rights Reserved


#include "UI/Widgets/Composite/ProjectN_Leaf_Labeled.h"

#include "Components/TextBlock.h"

void UProjectN_Leaf_Labeled::SetLabelText(const FText& NewLabelText, bool bCollapse) const
{
	if (bCollapse)
	{
		Label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	Label->SetText(NewLabelText);
}

void UProjectN_Leaf_Labeled::SetValueText(const FText& NewValueText, bool bCollapse) const
{
	if (bCollapse)
	{
		Label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	Value->SetText(NewValueText);
}
