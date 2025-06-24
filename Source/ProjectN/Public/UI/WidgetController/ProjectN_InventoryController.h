// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_InventoryController.generated.h"

class UProjectN_EquipmentWidget;
class UProjectN_InventoryWidget;
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

	UFUNCTION(BlueprintCallable)
	void SetInventoryWidgetRef(UProjectN_InventoryWidget* NewInventoryWidget);
	
	UFUNCTION(BlueprintCallable)
	void SetEquipmentWidgetRef(UProjectN_EquipmentWidget* NewEquipmentWidget);

	/*********************************
	 *  Items manage
	 *********************************/
	UFUNCTION(BlueprintCallable)
	void TryAddItem(const FName ItemID, const int32 Quantity) const;

	UFUNCTION(BlueprintCallable)
	void TryAddItemToSlot(const FGuid BagIndex, const int32 SlotIndex, const FName& ItemID, const int32 Quantity) const;

	UFUNCTION(BlueprintCallable)
	void StackItems(const FName ItemID, const FGuid FromBagIndex, const FGuid ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex, const int32 QuantityToAdd);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(const FGuid FromBagIndex, const int32 FromSlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void ReplaceItemsInBag(const FGuid FromBagIndex, const FGuid ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex) const;
	
	/*********************************
	*  Equipping manage
	*********************************/
	UFUNCTION(BlueprintCallable)
	void EquipItemToSlot(const FName ItemID, const EEquipSlot ToSlot, const int32 ItemStack = 1) const;

	UFUNCTION(BlueprintCallable)
	void UnEquipSlot(const EEquipSlot Slot) const;
	
protected:
	/*******************
	*   Getters
	********************/
	FEntriesDefinition* GetEntryManifest(const FName& ItemID) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> Entries;

private:

	UPROPERTY()
	TObjectPtr<UProjectN_InventoryComponent> InventoryComponent;
	UPROPERTY()
	TObjectPtr<UProjectN_InventoryWidget> InventoryWidget;
	UPROPERTY()
	TObjectPtr<UProjectN_EquipmentWidget> EquipmentWidget;
};
