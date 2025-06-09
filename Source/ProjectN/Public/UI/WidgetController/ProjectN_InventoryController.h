// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_InventoryController.generated.h"

struct FGameplayTag;
class UProjectN_InventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateBagSignature, FBagData, BagData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateSlotSignature, FInventorySlotData, ItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateEquipSlotSignature, FEquipSlotData, ItemData);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_InventoryController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Bag Info")
	FOnUpdateBagSignature OnUpdateBag;
	UPROPERTY(BlueprintAssignable, Category="Bag Info")
	FOnUpdateSlotSignature OnUpdateInventorySlot;
	UPROPERTY(BlueprintAssignable, Category="Equip Info")
	FOnUpdateEquipSlotSignature OnUpdateEquipSlot;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

	/*********************************
	 *  Items manage
	 *********************************/
	UFUNCTION(BlueprintCallable)
	void TryAddItem(const FName ItemID, const EEntryType ItemType, const int32 Quantity) const;

	UFUNCTION(BlueprintCallable)
	void AddStackToItem(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex, const int32 QuantityToAdd);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(const int32 FromBagIndex, const int32 FromSlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void ReplaceItemsInBag(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex) const;

	/*********************************
	*  Equipping manage
	*********************************/
	UFUNCTION(BlueprintCallable)
	void EquipItemToSlot(const FName ItemID, const EEntryType ItemType, const EEquipSlot ToSlot) const;

	UFUNCTION(BlueprintCallable)
	void UnEquipSlot(const EEquipSlot Slot) const;
	
protected:

	void LoadItemIconFromStruct(FInventorySlotData& InventorySlotData) const;
	void LoadItemIconFromStruct(FEquipSlotData& EquippedSlotData) const;
	void LoadIcon(const FName& ItemID, const EEntryType ItemType, UTexture2D*&  ItemIcon) const;

	/*******************
	*   Getters
	********************/
	const FItemDefinition* GetItemData(const FName& ItemID) const;
	const FEquippableItemDefinition* GetEquippableItemData(const FName& ItemID) const;
	const FWeaponItemDefinition* GetWeaponData(const FName& ItemID) const;
	const FBagDefinition* GetBagData(const FName& ItemID) const;
	
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

private:
	TObjectPtr<UProjectN_InventoryComponent> InventoryComponent;
};
