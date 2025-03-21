// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectN_AbilitySystemLibrary.generated.h"

class UProjectN_AttributeController;
class UProjectN_OverlayWidgetController;
class UProjectN_InventoryController;
/**
 * 
 */
UCLASS()
class PROJECTN_API UProjectN_AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AbilitySystem | Overlay Widget Controller")
	static UProjectN_OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem | Attribure Widget Controller")
	static UProjectN_AttributeController* GetAttributeWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem | Inventory Widget Controller")
	static UProjectN_InventoryController* GetInventoryWidgetController(const UObject* WorldContextObject);
};
