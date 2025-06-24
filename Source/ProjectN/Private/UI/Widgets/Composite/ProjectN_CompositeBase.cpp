// N Chedurov All Rights Reserved


#include "UI/Widgets/Composite/ProjectN_CompositeBase.h"

void UProjectN_CompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UProjectN_CompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
