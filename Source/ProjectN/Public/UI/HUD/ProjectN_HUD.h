// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "UI/WidgetController/ProjectN_AttributeController.h"
#include "UI/WidgetController/ProjectN_InventoryController.h"
#include "UI/WidgetController/ProjectN_MainMenuWidgetController.h"
#include "UI/WidgetController/ProjectN_OverlayWidgetController.h"
#include "UI/WidgetController/ProjectN_SaveGameWidgetController.h"
#include "ProjectN_HUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;

UCLASS()
class PROJECTN_API AProjectN_HUD : public AHUD
{
	GENERATED_BODY()

public:

	UProjectN_OverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UProjectN_AttributeController* GetAttributeWidgetController(const FWidgetControllerParams& WCParams);
	UProjectN_InventoryController* GetInventoryWidgetController(const FWidgetControllerParams& WCParams);
//	UProjectN_MainMenuWidgetController* GetMainMenuWidgetController(const FWidgetControllerParams& WCParams);
	UProjectN_SaveGameWidgetController* GetSaveGameWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

	void BroadcastValues();

	// Widgets
	UPROPERTY()
	TObjectPtr<UProjectN_WidgetBase> OverlayWidget;

	// Widget classes
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_WidgetBase> OverlayWidgetClass;

	// Main menu widget controller
//	UPROPERTY(EditAnywhere)
//	TSubclassOf<UProjectN_MainMenuWidgetController> MainMenuWidgetControllerClass;
//	UPROPERTY()
//	TObjectPtr<UProjectN_MainMenuWidgetController> MainMenuWidgetController;
	
	// Overlay widget controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_OverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UProjectN_OverlayWidgetController> OverlayWidgetController;

	// Attribute widget controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_AttributeController> AttributeWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UProjectN_AttributeController> AttributeWidgetController;

	// Main menu widget controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_SaveGameWidgetController> SaveGameWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UProjectN_SaveGameWidgetController> SaveGameWidgetController;
	
	// Inventory widget controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_InventoryController> InventoryWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UProjectN_InventoryController> InventoryWidgetController;
};