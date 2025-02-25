// N Chedurov All Rights Reserved


#include "UI/HUD/ProjectN_HUD.h"

#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"

UProjectN_OverlayWidgetController* AProjectN_HUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UProjectN_OverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToResponce();

		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}

void AProjectN_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized, please fill out HUD data"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized, please fill out BP_HUD"))
	
	OverlayWidget = CreateWidget<UProjectN_WidgetBase>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UProjectN_OverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	
	OverlayWidget->AddToViewport();
}
