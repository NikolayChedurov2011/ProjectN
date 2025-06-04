// N Chedurov All Rights Reserved


#include "UI/HUD/ProjectN_HUD.h"

#include "UI/WidgetController/ProjectN_AbilityBookController.h"
#include "UI/WidgetController/ProjectN_ActionBarController.h"
#include "UI/WidgetController/ProjectN_AttributeController.h"
#include "UI/WidgetController/ProjectN_InventoryController.h"
//#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"

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

UProjectN_AttributeController* AProjectN_HUD::GetAttributeWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeWidgetController == nullptr)
	{
		AttributeWidgetController = NewObject<UProjectN_AttributeController>(this, AttributeWidgetControllerClass);
		AttributeWidgetController->SetWidgetControllerParams(WCParams);
		AttributeWidgetController->BindCallbacksToResponce();

		return AttributeWidgetController;
	}

	return AttributeWidgetController;
}

UProjectN_InventoryController* AProjectN_HUD::GetInventoryWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UProjectN_InventoryController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToResponce();

		return InventoryWidgetController;
	}

	return InventoryWidgetController;
}

UProjectN_ActionBarController* AProjectN_HUD::GetActionBarWidgetController(const FWidgetControllerParams& WCParams)
{
	if (ActionBarWidgetController == nullptr)
	{
		ActionBarWidgetController = NewObject<UProjectN_ActionBarController>(this, ActionBarWidgetControllerClass);
		ActionBarWidgetController->SetWidgetControllerParams(WCParams);
		ActionBarWidgetController->BindCallbacksToResponce();

		return ActionBarWidgetController;
	}

	return ActionBarWidgetController;
}

UProjectN_AbilityBookController* AProjectN_HUD::GetAbilityBookWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AbilityBookWidgetController == nullptr)
	{
		AbilityBookWidgetController = NewObject<UProjectN_AbilityBookController>(this, AbilityBookWidgetControllerClass);
		AbilityBookWidgetController->SetWidgetControllerParams(WCParams);
		AbilityBookWidgetController->BindCallbacksToResponce();

		return AbilityBookWidgetController;
	}

	return AbilityBookWidgetController;
}

void AProjectN_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized, please fill out HUD data"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized, please fill out BP_HUD"))
	
	OverlayWidget = CreateWidget<UProjectN_WidgetBase>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	AttributeWidgetController = GetAttributeWidgetController(WidgetControllerParams);
	InventoryWidgetController = GetInventoryWidgetController(WidgetControllerParams);
	ActionBarWidgetController = GetActionBarWidgetController(WidgetControllerParams);
	AbilityBookWidgetController = GetAbilityBookWidgetController(WidgetControllerParams);
	
	OverlayWidget->AddToViewport();
}

void AProjectN_HUD::BroadcastValues()
{
	OverlayWidgetController->BroadcastInitialValues();
	AttributeWidgetController->BroadcastInitialValues();
}
