// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

enum class EItemSlot : uint8;
struct FGameplayTag;
class UWeaponItemStaticClass;
class AProjectN_ItemActor_Base;

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
	float GetWeaponMinDamageForSlot(const EItemSlot ItemSlot) const;

	UFUNCTION(BlueprintNativeEvent)
	float GetWeaponMaxDamageForSlot(const EItemSlot ItemSlot) const;

	UFUNCTION(BlueprintNativeEvent)
	AProjectN_ItemActor_Base* GetEquippedWeaponForSlot(const EItemSlot ItemSlot) const;
};
