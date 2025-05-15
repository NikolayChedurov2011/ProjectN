// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectN_AbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UCharacter_Save;
class USaveGame;
class UAbilitySystemComponent;
class UProjectN_AttributeController;
class UProjectN_OverlayWidgetController;
class UProjectN_InventoryController;
	
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

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem | Attributes")
	static void SetPrimaryAttributesByCaller(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, const float Strength, const float Intelligence, const float Dexterity, const float Vitality);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem | Gameplay Effects")
	static bool IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem | Gameplay Effects")
	static void SetIsBlock(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bBlocked);

	UFUNCTION(BlueprintPure, Category = "AbilitySystem | Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem | Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bCritical);
};