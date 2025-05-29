// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_InventoryController.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateBagSignature, const int32, BagID, const int32, BagSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateSlotSignature, const int32, BagID, const int32, BagSlots, FInventorySlotData, ItemData);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_InventoryController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Bag Info")
	FOnUpdateBagSignature OnUpdateBag;
	UPROPERTY(BlueprintAssignable, Category="Bag Info")
	FOnUpdateSlotSignature OnUpdateSlot;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> EquipmentItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> WeaponItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> AbilityDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> BagDataTable;
};
