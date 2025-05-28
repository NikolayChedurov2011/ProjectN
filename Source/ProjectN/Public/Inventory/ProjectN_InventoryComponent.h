// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "Inventory/PojectN_InventoryItemsRecord.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_InventoryComponent.generated.h"

class UProjectN_LootDataAsset;

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
	FEquippedItemData(UProjectN_ItemInstance* InItemInstance, const EItemSlot InItemSlot) : ItemInstance(InItemInstance), ItemSlot(InItemSlot) {}

	UPROPERTY()
	UProjectN_ItemInstance* ItemInstance = nullptr;

	UPROPERTY()
	EItemSlot ItemSlot = EItemSlot::None;
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

DECLARE_DELEGATE_OneParam(FOnUpdateItemSignature, UProjectN_ItemInstance* /*NewItem*/);

DECLARE_DELEGATE_TwoParams(FOnBagChangedSignature, const int32 /*BagID*/, const int32 /*BagSlot*/);
DECLARE_DELEGATE_ThreeParams(FOnSlotChangeSignature, const int32 /*BagID*/, const int32 /*BagSlot*/, FInventorySlotData /*ItemData*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Default bag id
	
	FOnUpdateItemSignature OnUpdateItem;
	FOnUpdateItemSignature OnRemoveItem;
	
	UProjectN_InventoryComponent();
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void AddItemByStaticClass(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass, const int32 ItemStack);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItemByInstance(UProjectN_ItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void EquipItemByInstance(UProjectN_ItemInstance* InItemInstance, const EItemSlot InSlot);
	
	UFUNCTION(BlueprintCallable)
	void UnEquipItemByInstance(UProjectN_ItemInstance* InItemInstance);
	
	UFUNCTION(BlueprintCallable)
	void DropItem(UProjectN_ItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	bool IsEquippableItem(UProjectN_ItemInstance* InItemInstance) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE  TArray<FInventoryItem>& GetItemsList() { return InventoryList.GetItemsRef(); }

	/*********************
	 *   Equippable items
	 *********************/
	FVector FindSocketLocationBySlot(const EItemSlot ItemSlot);

	/*********************
	 *   Weapon items
	 *********************/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AProjectN_WeaponActor* GetEquippedWeaponActorBySlot(const EItemSlot InItemSlot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FGameplayTag, float> GetWeaponDamageTypesForSlot(const EItemSlot InItemSlot);

	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//float GetWeaponMaxDamageForSlot(const EItemSlot InItemSlot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetWeaponTypeBySlot(const EItemSlot InItemSlot);
	
	//virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	/******************************
	 *   For inventory controller
	 ******************************/
	UFUNCTION(Client, Reliable)
	void ClientUpdateItemInfo(UProjectN_ItemInstance* ItemInstance);
	
	UFUNCTION(Client, Reliable)
	void ClientRemoveItem(UProjectN_ItemInstance* ItemInstance);

	/*
	USTRUCT()
	struct FActionSlotData
	{
		GENERATED_BODY()
		int32 ID;                         // 101, 201, 301, 401…
		EItemEntryType EntryType;        // Spell, Consumable, Equipment, Weapon
		int32 BagID, SlotIndex;          // только для айтемов
	};
	USTRUCT()
	struct FInventorySlotData
	{
		GENERATED_BODY()
		int32 ItemID;
		EItemEntryType EntryType;        // однозначно говорит, в какую таблицу копать
		int32 Quantity;
		float Durability;
		// …
	};
	*/
	
protected:
	virtual void InitializeComponent() override;

	//UFUNCTION()
	//void AddInventoryTags();

	//void HandleGameplayEventInternal(const FGameplayEventData Payload);

	//UFUNCTION(Server, Reliable)
	//void ServerHandleGameplayEvent(const FGameplayEventData Payload);
	
	/******************
	 *  Slots managing
	 ******************/
	bool IsSlotEquipped(const EItemSlot InItemSlot);
	bool IsSlotEquipped(const UProjectN_ItemInstance* InItemInstance);
	FEquippedItemData* FindItemDataBySlot(const EItemSlot InItemSlot);
	FEquippedItemData* FindItemDataByInstance(const UProjectN_ItemInstance* InItemInstance);
	void RemoveSlot(const EItemSlot InItemSlot);
	void AddItemToSlot(const EItemSlot InItemSlot, UProjectN_ItemInstance* InItemInstance);

	/********************************
	 *  Weapon abilities managing
	 ********************************/
	void UpdateWeaponMode();
	void RemoveWeaponAbilities(const EWeaponMode WeaponMode);
	void GiveWeaponAbilities(UItemStaticClass* WeaponItemStaticClass, const EWeaponMode WeaponMode, const bool bAddForMainHand, const bool bAddForAuxiliaryHand);
	void ApplyItemStats(const UProjectN_ItemInstance* InItem) const;
	void RemoveItemStats(const UProjectN_ItemInstance* InItem) const;
	
	/******************
	 *  Debug functions
	 ******************/
	void PrintMessage(const FString& InText);

	UPROPERTY(Replicated, EditDefaultsOnly)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectN_LootDataAsset> DefaultLootData;

	UPROPERTY(Replicated)
	FEquippedItemsList EquippedItemSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, EItemSlot> AssociatedInputTagWithSlot;

	/*UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> EquipmentItemsDataTable;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> WeaponItemsDataTable;*/



	//////////////////////////////////////////////////////
	// Wow realisation
public:
	FOnBagChangedSignature OnBagChanged;
	//FOnBagChangedSignature OnBagAdded;
	//FOnBagChangedSignature OnBagRemoved;
	FOnSlotChangeSignature OnSlotChange;

	UFUNCTION(BlueprintCallable)
	void AddBag(const FName InBagItemID);
	UFUNCTION(BlueprintCallable)
	void RemoveBag(const int32 BagID);
	UFUNCTION(BlueprintCallable)
	bool TryAddItemToFirstFreeSlot(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	UFUNCTION(BlueprintCallable)
	bool TryAddItemToStack(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	UFUNCTION(BlueprintCallable)
	void ReplaceItemInBag(const int32 FromBagID, const int32 ToBagID, const int32 FromSlotIndex, const int32 ToSlotIndex);

	UFUNCTION(Client, Reliable)
	void BroadcastBagChange(const int32 BagID, const int32 BagSlots);
	UFUNCTION(Client, Reliable)
	void BroadcastSlotChange(const int32 BagID, const int32 BagSlot, const FInventorySlotData& ItemData);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FBagData> GetBags() const { return Bags; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="WOW Realisation")
	TSoftObjectPtr<UDataTable> BagDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WOW Realisation")
	TArray<FName> BagsDefaultID;

	UPROPERTY(Replicated)
	TArray<FBagData> Bags;

	void InitBags();

	FInventorySlotData& GetSlot(const int32 BagID, const int32 SlotIndex)
	{
		check(Bags.IsValidIndex(BagID));
		check(Bags[BagID].Slots.IsValidIndex(SlotIndex));
		return Bags[BagID].Slots[SlotIndex];
	}

private:
	
	TMap<EWeaponMode, FGrantedAbilityHandles> GrantedAbilityHandlesByMode;
	TMap<EItemSlot, FGrantedAbilityHandles> GrantedEffectHandlesByInstance;
};
