// N Chedurov All Rights Reserved


#include "UI/Widgets/ProjectN_WidgetBase.h"

void UProjectN_WidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
