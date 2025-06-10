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
struct FEquippedItemsList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FEquipSlotData> EquippedItems;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& Delta)
	{
		return FastArrayDeltaSerialize<FEquipSlotData, FEquippedItemsList>(EquippedItems, Delta, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FEquippedItemsList> : public TStructOpsTypeTraitsBase2<FEquippedItemsList>
{
	enum { WithNetDeltaSerializer = true };
};

DECLARE_DELEGATE_OneParam(FOnBagChangedSignature, FBagData /*BagData*/);
DECLARE_DELEGATE_OneParam(FOnSlotChangeSignature, FInventorySlotData /*ItemData*/);
DECLARE_DELEGATE_OneParam(FOnEquipSlotChangeSignature, FEquipSlotData /*SlotData*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FOnBagChangedSignature OnBagChanged;
	FOnSlotChangeSignature OnInventorySlotChange;
	FOnEquipSlotChangeSignature OnEquipSlotChange;
	
	UProjectN_InventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
	/*********************************
	 *  Getters
	 *********************************/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FBagData> GetBags() const { return BagList.Bags; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AProjectN_WeaponActor* GetEquippedWeaponActorBySlot(const EEquipSlot InItemSlot);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetWeaponTypeBySlot(const EEquipSlot InItemSlot) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FGameplayTag, float> GetWeaponDamageTypesForSlot(const EEquipSlot InItemSlot) const;

	FVector FindWeaponSocketLocationForProjectileBySlot(const EEquipSlot ItemSlot) const;

	FInventorySlotData& GetSlot(const int32 BagID, const int32 SlotIndex)
	{
		check(BagList.Bags.IsValidIndex(BagID));
		check(BagList.Bags[BagID].Slots.IsValidIndex(SlotIndex));
		return BagList.Bags[BagID].Slots[SlotIndex];
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsTwoHandedEquip() { return bIsTwoHandedEquip; }
	
	/*********************************
	 *  Bag manage
	 *********************************/
	UFUNCTION(BlueprintCallable)
	void AddBag(const FName InBagItemID);
	
	UFUNCTION(BlueprintCallable)
	void RemoveBag(const int32 BagIndex);

	
	/*********************************
	 *  Items manage
	 *********************************/
	UFUNCTION(Server, Reliable)
	void ServerTryAddItem(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);

	UFUNCTION(Server, Reliable)
	void ServerAddStackToItem(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex, const int32 QuantityToAdd);
	
	UFUNCTION(Server, Reliable)
	void ServerRemoveItem(const int32 FromBagIndex, const int32 FromSlotIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerReplaceItemInBag(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex);

	UFUNCTION(Server, Reliable)
	void ServerTryUseItem(const FName& ItemID, const EEntryType ItemType);

	
	/*********************************
	 *  Equipping manage
	 *********************************/
	UFUNCTION(Server, Reliable)
	void ServerEquipItemToSlot(const FName& ItemID, const EEntryType ItemType, const EEquipSlot ToSlot);

	UFUNCTION(Server, Reliable)
	void ServerUnEquipSlot(const EEquipSlot Slot);
	void UnEquipSlotAndReturnWeapon(const EEquipSlot Slot);

	
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
	bool IsSlotEquipped(const EEquipSlot Slot);
	const FEquipSlotData* GetEquippedSlotData(const FName& ItemID, const EEntryType ItemType) const;
	const FEquipSlotData* GetEquippedSlotData(const EEquipSlot ItemSlot) const;
	const FEquippableItemDefinition* GetEquipmentData(const EEquipSlot ItemSlot) const;
	const FEquippableItemDefinition* GetEquippableItemData(const FName& ItemID) const;
	const FWeaponItemDefinition* GetEquippedWeaponData(const EEquipSlot ItemSlot) const;
	const FWeaponItemDefinition* GetWeaponData(const FName& ItemID) const;
	const FItemDefinition* GetItemData(const FName& ItemID) const;

	FORCEINLINE void SetIsTwoHandedEquip(bool bIsTwoHanded) { bIsTwoHandedEquip = bIsTwoHanded; }

	/*********************************
	 *  Bag manage
	 *********************************/
	void InitBags();

	/*********************************
	 *  Items manage
	 *********************************/
	bool TryAddItemToFirstFreeSlot(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	void TryAddItemToStack(const FName& ItemID, const EEntryType ItemType, const int32 Quantity);
	
	/*********************************
	 *  Equipping manage
	 *********************************/
	AActor* SpawnItemActor(const FName& ItemID, const EEntryType ItemType, const EEquipSlot EItemSlot, AActor* Owner) const;

	UFUNCTION(Client, Reliable)
	void ClientUseNewAnimInstance(const FName& ItemID);

	UFUNCTION(Client, Reliable)
	void ClientResetAnimInstance();
	void SetAnimInstance(const TSubclassOf<UAnimInstance>& AnimInstance) const;

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
	void BroadcastBagChange(const FBagData& BagData);
	UFUNCTION(Client, Reliable)
	void BroadcastSlotChange(const FInventorySlotData& ItemData);
	UFUNCTION(Client, Reliable)
	void BroadcastEquipSlotChange(const FEquipSlotData& EquipSlotData);

	
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
	TMap<FGameplayTag, EEquipSlot> AssociatedInputTagWithSlot;
	
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

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Animation Data")
	TSubclassOf<UAnimInstance> DefaultAnimInstance = nullptr;

	TMap<EWeaponMode, FGrantedAbilityHandles> GrantedAbilityHandlesByMode;
	TMap<EEquipSlot, FGrantedAbilityHandles> GrantedEffectHandlesByInstance;

	UPROPERTY(Replicated)
	bool bIsTwoHandedEquip = false;
};
