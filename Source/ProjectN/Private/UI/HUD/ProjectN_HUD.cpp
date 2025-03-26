// N Chedurov All Rights Reserved


#include "UI/HUD/ProjectN_HUD.h"

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

UProjectN_MainMenuWidgetController* AProjectN_HUD::GetMainMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (MainMenuWidgetController == nullptr)
	{
		MainMenuWidgetController = NewObject<UProjectN_MainMenuWidgetController>(this, MainMenuWidgetControllerClass);
		MainMenuWidgetController->SetWidgetControllerParams(WCParams);
		MainMenuWidgetController->BindCallbacksToResponce();

		return MainMenuWidgetController;
	}

	return MainMenuWidgetController;
}

UProjectN_SaveGameWidgetController* AProjectN_HUD::GetSaveGameWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SaveGameWidgetController == nullptr)
	{
		SaveGameWidgetController = NewObject<UProjectN_SaveGameWidgetController>(this, SaveGameWidgetControllerClass);
		SaveGameWidgetController->SetWidgetControllerParams(WCParams);
		SaveGameWidgetController->BindCallbacksToResponce();

		return SaveGameWidgetController;
	}

	return SaveGameWidgetController;
}

void AProjectN_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized, please fill out HUD data"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized, please fill out BP_HUD"))
	
	OverlayWidget = CreateWidget<UProjectN_WidgetBase>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	AttributeWidgetController = GetAttributeWidgetController(WidgetControllerParams);
	MainMenuWidgetController = GetMainMenuWidgetController(WidgetControllerParams);
	
	OverlayWidget->AddToViewport();
}

void AProjectN_HUD::BroadcastValues()
{
	OverlayWidgetController->BroadcastInitialValues();
	AttributeWidgetController->BroadcastInitialValues();
}
