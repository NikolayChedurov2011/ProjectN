// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectN_HUD.generated.h"

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

	UFUNCTION(BlueprintCallable)
	UProjectN_OverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	UPROPERTY()
	TObjectPtr<UProjectN_WidgetBase> OverlayWidget;

protected:

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_WidgetBase> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UProjectN_OverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UProjectN_OverlayWidgetController> OverlayWidgetController;
};
