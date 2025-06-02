// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "InventoryInterface.generated.h"

enum class EEquipSlot : uint8;
class UWeaponItemStaticClass;
class AProjectN_WeaponActor;

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTN_API IInventoryInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	TMap<FGameplayTag, float> GetWeaponDamageTypes(const EEquipSlot ItemSlot) const;

	//UFUNCTION(BlueprintNativeEvent)
	//float GetWeaponMaxDamageForSlot(const EItemSlot ItemSlot) const;

	UFUNCTION(BlueprintNativeEvent)
	AProjectN_WeaponActor* GetEquippedWeaponForSlot(const EEquipSlot ItemSlot) const;
};
