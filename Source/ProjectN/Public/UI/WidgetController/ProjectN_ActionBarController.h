// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_ActionBarController.generated.h"

class UProjectN_ActionSlot;
class UProjectN_ActionBartWidget;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateActionSlotSignature, const FActionSlotData, ActionSlotInfo);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_ActionBarController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Action Slot Info")
	FOnUpdateActionSlotSignature OnUpdateActionSlot;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

	void SwapActionSlots(const int32 ToSlotIndex, const int32 FromSlotIndex, const FName& IncomingItemID) const;
	void UpdateActionSlot(const int32 ActionSlotIndex, const FName& IncomingItemID) const;
	void ClearActionSlot(const int32 ActionSlotIndex) const;
	
	/*UFUNCTION(BlueprintCallable)
	void ClearSlot(const int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void AddSlot(FActionSlotData ActionSlotInfo);*/

	UFUNCTION(BlueprintCallable)
	void SetActionBarWidgetRef(UProjectN_ActionBartWidget* NewActionBarWidget);
	
protected:

	/*void LoadItemIcon(FActionSlotData& ActionSlotInfo) const;
	void AddAbility(const FActionSlotData& ActionSlotInfo) const;
	void RemoveAbility(const FGameplayTag InputActionTag) const;
	
	const FAbilityDefinition* GetAbilityData(const FName& ItemID) const;
	const FItemDefinition* GetItemData(const FName& ItemID) const;
	const FConsumableItemDefinition* GetConsumableItemData(const FName& ItemID) const;
	const FEquippableItemDefinition* GetEquippableItemData(const FName& ItemID) const;
	const FWeaponItemDefinition* GetWeaponData(const FName& ItemID) const;
	const FBagDefinition* GetBagData(const FName& ItemID) const;

	void FindCooldownTagFromStruct(FActionSlotData& InventorySlotData) const;
	
	TArray<FActionSlotData> ActionSlots;*/
	
/*******************
*   Getters
********************/
	FEntriesDefinition* GetEntryManifest(const FName& ItemID) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TObjectPtr<UDataTable> Entries;

private:

	UPROPERTY()
	TObjectPtr<UProjectN_ActionBartWidget> ActionBarWidget;
};
