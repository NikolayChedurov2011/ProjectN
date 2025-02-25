// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"

void UProjectN_WidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UProjectN_WidgetControllerBase::BroadcastInitialValues()
{
	
}

void UProjectN_WidgetControllerBase::BindCallbacksToResponce()
{
	
}
