// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_InventoryController.generated.h"

struct FGameplayTag;
/*class UProjectN_ItemInstance;

USTRUCT(BlueprintType)
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FString ItemName = FString();

	UPROPERTY(BlueprintReadOnly)
	FString ItemDescription = FString();

	UPROPERTY(BlueprintReadOnly)
	int32 ItemStack = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bCanBeEquipped = false;

	UPROPERTY(BlueprintReadOnly)
	UProjectN_ItemInstance* ItemInstance = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemsSignature, const TArray<FInventoryItemInfo>&, ItemsInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemAddedSignature, const FInventoryItemInfo&, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemRemovedSignature, const UProjectN_ItemInstance*, ItemInfo);*/


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateBagSignature, const int32, BagID, const int32, BagSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateSlotSignature, const int32, BagID, const int32, BagSlots, FInventorySlotData, ItemData);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_InventoryController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	/*UPROPERTY(BlueprintAssignable, Category="Item Info")
	FOnInventoryItemsSignature OnInventoryItems;

	UPROPERTY(BlueprintAssignable, Category="Item Info")
	FOnInventoryItemAddedSignature OnInventoryItemAdded;
	
	UPROPERTY(BlueprintAssignable, Category="Item Info")
	FOnInventoryItemRemovedSignature OnInventoryItemRemoved;*/

	UPROPERTY(BlueprintAssignable, Category="Bag Info")
	FOnUpdateBagSignature OnUpdateBag;
	UPROPERTY(BlueprintAssignable, Category="Bag Info")
	FOnUpdateSlotSignature OnUpdateSlot;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="WOW Realisation")
	TSoftObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="WOW Realisation")
	TSoftObjectPtr<UDataTable> EquipmentItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="WOW Realisation")
	TSoftObjectPtr<UDataTable> WeaponItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="WOW Realisation")
	TSoftObjectPtr<UDataTable> AbilityDataTable;

	UPROPERTY(EditDefaultsOnly, Category="WOW Realisation")
	TSoftObjectPtr<UDataTable> BagDataTable;
};
