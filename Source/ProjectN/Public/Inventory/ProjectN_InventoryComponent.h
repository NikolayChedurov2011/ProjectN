// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_InventoryComponent.generated.h"

class AProjectN_WeaponActor;

struct FGrantedAbilityHandles
{
	TArray<FGameplayAbilitySpecHandle> AbilitySpecs;
	TArray<FActiveGameplayEffectHandle>   EffectHandles;
};

USTRUCT()
struct FEquippedItemData : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FEquippedItemData(){}
	FEquippedItemData(const FName NewItemID, const EEntryType NewItemType, const EItemSlot NewSlot) : ItemID(NewItemID), ItemType(NewItemType), ItemSlot(NewSlot) {}

	UPROPERTY()
	FName ItemID = NAME_None;

	UPROPERTY()
	EEntryType ItemType = EEntryType::None;

	UPROPERTY()
	EItemSlot ItemSlot = EItemSlot::None;

	UPROPERTY()
	AActor* SpawnedActor = nullptr;
};

USTRUCT()
struct FEquippedItemsList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FEquippedItemData> EquippedItems;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& Delta)
	{
		return FastArrayDeltaSerialize<FEquippedItemData, FEquippedItemsList>(EquippedItems, Delta, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FEquippedItemsList> : public TStructOpsTypeTraitsBase2<FEquippedItemsList>
{
	enum { WithNetDeltaSerializer = true };
};

DECLARE_DELEGATE_TwoParams(FOnBagChangedSignature, const int32 /*BagID*/, const int32 /*BagSlot*/);
DECLARE_DELEGATE_ThreeParams(FOnSlotChangeSignature, const int32 /*BagID*/, const int32 /*BagSlot*/, FInventorySlotData /*ItemData*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FOnBagChangedSignature OnBagChanged;
	FOnSlotChangeSignature OnSlotChange;
	
	UProjectN_InventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
	/*********************************
	 *  Getters
	 *********************************/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FBagData> GetBags() const { return BagList.Bags; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AProjectN_WeaponActor* GetEquippedWeaponActorBySlot(const EItemSlot InItemSlot);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetWeaponTypeBySlot(const EItemSlot InItemSlot) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FGameplayTag, float> GetWeaponDamageTypesForSlot(const EItemSlot InItemSlot) const;

	FVector FindWeaponSocketLocationForProjectileBySlot(const EItemSlot ItemSlot) const;

	FInventorySlotData& GetSlot(const int32 BagID, const int32 SlotIndex)
	{
		check(BagList.Bags.IsValidIndex(BagID));
		check(BagList.Bags[BagID].Slots.IsValidIndex(SlotIndex));
		return BagList.Bags[BagID].Slots[SlotIndex];
	}

	
	/*********************************
	 *  Bag manage
	 *********************************/
	UFUNCTION(BlueprintCallable)
	void AddBag(const FName InBagItemID);
	
	UFUNCTION(BlueprintCallable)
	void RemoveBag(const int32 BagID);

	
	/*********************************
	 *  Items manage
	 *********************************/
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	
	UFUNCTION(BlueprintCallable)
	bool TryAddItemToFirstFreeSlot(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	
	UFUNCTION(BlueprintCallable)
	bool TryAddItemToStack(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(const int32 FromBagID, const int32 FromSlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void ReplaceItemInBag(const int32 FromBagID, const int32 ToBagID, const int32 FromSlotIndex, const int32 ToSlotIndex);

	
	/*********************************
	 *  Equipping manage
	 *********************************/
	UFUNCTION(BlueprintCallable)
	void EquipItemToSlot(const FName& ItemID, const EEntryType ItemType, const EItemSlot ToSlot);

	UFUNCTION(BlueprintCallable)
	void UnEquipSlot(const EItemSlot Slot);

	
	/*********************************
	 *  Abilities and stats managing
	 *********************************/
	
	
	/***********************************
	 *  Broadcast to widget controller
	 ***********************************/
	


	
	/******************************
	 *   For inventory controller
	 ******************************/
	
	
protected:
	virtual void InitializeComponent() override;

	/*********************************
	 *  Getters
	 *********************************/
	bool IsSlotEquipped(const EItemSlot Slot);
	const FEquippedItemData* GetEquippedSlotData(const FName& ItemID, const EEntryType ItemType) const;
	const FEquippedItemData* GetEquippedSlotData(const EItemSlot ItemSlot) const;
	const FEquippableItemDefinition* GetEquipmentData(const EItemSlot ItemSlot) const;
	const FEquippableItemDefinition* GetEquippableItemData(const FName& ItemID) const;
	const FWeaponItemDefinition* GetEquippedWeaponData(const EItemSlot ItemSlot) const;
	const FWeaponItemDefinition* GetWeaponData(const FName& ItemID) const;

	/*********************************
	 *  Bag manage
	 *********************************/
	void InitBags();

	/*********************************
	 *  Items manage
	 *********************************/

	
	/*********************************
	 *  Equipping manage
	 *********************************/
	AActor* SpawnItemActor(const FName& ItemID, const EEntryType ItemType, const EItemSlot EItemSlot, AActor* Owner) const;


	/*********************************
	 *  Abilities and stats managing
	 *********************************/
	void ApplyItemStats(const FName& ItemID, const EEntryType ItemType) const;
	void RemoveItemStats(const FName& ItemID, const EEntryType ItemType) const;
	void GiveWeaponAbilities(const FWeaponItemDefinition* WeaponItemDefinition, const EWeaponMode WeaponMode, const bool bAddForMainHand, const bool bAddForAuxiliaryHand);
	void RemoveWeaponAbilities(const EWeaponMode WeaponMode);
	void UpdateWeaponMode();

	
	/***********************************
	 *  Broadcast to widget controller
	 ***********************************/
	UFUNCTION(Client, Reliable)
	void BroadcastBagChange(const int32 BagID, const int32 BagSlots);
	UFUNCTION(Client, Reliable)
	void BroadcastSlotChange(const int32 BagID, const int32 BagSlot, const FInventorySlotData& ItemData);

	
	/******************
	 *  Debug functions
	 ******************/
	void PrintMessage(const FString& InText) const;

private:

//	UPROPERTY(Replicated)
//	TArray<FBagData> Bags;

	UPROPERTY(Replicated)
	FBagList BagList;
	
	UPROPERTY(Replicated)
	FEquippedItemsList EquippedSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TMap<FGameplayTag, EItemSlot> AssociatedInputTagWithSlot;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Table")
	TArray<FName> BagsDefaultID;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Table")
	TSoftObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Tablen")
	TSoftObjectPtr<UDataTable> EquippableItemDataTable;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Table")
	TSoftObjectPtr<UDataTable> WeaponDataTable;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Table")
	TSoftObjectPtr<UDataTable> BagDataTable;

	TMap<EWeaponMode, FGrantedAbilityHandles> GrantedAbilityHandlesByMode;
	TMap<EItemSlot, FGrantedAbilityHandles> GrantedEffectHandlesByInstance;
};
