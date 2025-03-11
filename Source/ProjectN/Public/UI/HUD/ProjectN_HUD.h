// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectN_HUD.generated.h"

class UProjectN_AttributeController;
class UAttributeSet;
class UAbilitySystemComponent;
class UProjectN_OverlayWidgetController;
class UProjectN_WidgetBase;
struct FWidgetControllerParams;

UCLASS()
class PROJECTN_API AProjectN_HUD : public AHUD
{
	GENERATED_BODY()

public:

	UProjectN_OverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UProjectN_AttributeController* GetAttributeWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:

	// Widgets
	UPROPERTY()
	TObjectPtr<UProjectN_WidgetBase> OverlayWidget;

	// Widget classes
	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_WidgetBase> OverlayWidgetClass;

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
};