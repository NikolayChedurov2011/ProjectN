// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_ActionBarController.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateActionSlotSignature, const FProjectNActionSlotInfo, ActionSlotInfo);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateBagSignature, const int32, BagID, const int32, BagSlots);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateSlotSignature, const int32, BagID, const int32, BagSlots, FInventorySlotData, ItemData);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_ActionBarController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

//	UPROPERTY(BlueprintAssignable, Category="Bag Info")
//	FOnUpdateBagSignature OnUpdateBag;
//	UPROPERTY(BlueprintAssignable, Category="Bag Info")
//	FOnUpdateSlotSignature OnUpdateSlot;

	UPROPERTY(BlueprintAssignable, Category="Action Slot Info")
	FOnUpdateActionSlotSignature OnUpdateActionSlot;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

	void AddAbility(const FProjectNActionSlotInfo& ActionSlotInfo) const;
	
protected:

	const FAbilityDefinition* GetAbilityData(const FName& ItemID) const;
	const FItemDefinition* GetItemData(const FName& ItemID) const;
	const FEquippableItemDefinition* GetEquippableItemData(const FName& ItemID) const;
	const FWeaponItemDefinition* GetWeaponData(const FName& ItemID) const;
	const FBagDefinition* GetBagData(const FName& ItemID) const;
	
	TArray<FProjectNActionSlotInfo> ActionSlots;
	
	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> EquippableItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> WeaponItemsDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> AbilityDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> BagDataTable;
};
