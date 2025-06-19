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

DECLARE_DELEGATE_OneParam(FOnBagChangedSignature, const FBagData& /*BagData*/);
DECLARE_DELEGATE_ThreeParams(FOnSlotChangeSignature, const int32 /*BagIndex*/, const int32 /*SlotIndex*/, const FInventorySlotData& /*ItemData*/);
DECLARE_DELEGATE_OneParam(FOnEquipSlotChangeSignature, const FEquipSlotData& /*SlotData*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	FOnBagChangedSignature OnBagChanged;
	FOnBagChangedSignature OnBagRemoved;
	FOnSlotChangeSignature OnInventorySlotChange;
	FOnSlotChangeSignature OnInventorySlotRemoved;
	FOnEquipSlotChangeSignature OnEquipSlotChange;
	FOnEquipSlotChangeSignature OnEquipSlotRemoved;
	
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

	FBagData* FindBagForSlot(const int32 BagIndexToFind);
	FInventorySlotData* GetBagSlot(const int32 BagIndex, const int32 SlotIndex);
	FInventorySlotData* FindItemInBag(const FName ItemID);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsTwoHandedEquip() { return bIsTwoHandedEquip; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UAnimSequence* GetCurrentTwoHandedPosture() { return CurrentTwoHandedPosture.Get(); }
	
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
	void ServerTryAddItem(const FName& ItemID, const int32 Quantity);
	
	UFUNCTION(Server, Reliable)
	void ServerTryAddItemToSlot(const int32 BagIndex, const int32 SlotIndex, const FName& ItemID, const int32 Quantity);

	UFUNCTION(Server, Reliable)
	void ServerStackItems(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex, const int32 QuantityToAdd, const int32 MaxStack);
	
	UFUNCTION(Server, Reliable)
	void ServerRemoveItem(const int32 FromBagIndex, const int32 FromSlotIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerReplaceItemInBag(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex);

	UFUNCTION(Server, Reliable)
	void ServerTryUseItem(const FName& ItemID);

	
	/*********************************
	 *  Equipping manage
	 *********************************/
	UFUNCTION(Server, Reliable)
	void ServerEquipItemToSlot(const FName& ItemID, const EEquipSlot ToSlot, const int32 ItemStack = 1);

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
	const FEquipSlotData* GetEquippedSlotData(const EEquipSlot ItemSlot) const;
	const FWeaponFragment* GetEquippedWeaponFragment(const EEquipSlot ItemSlot) const;
	FEntriesDefinition* GetEntryManifest(const FName& ItemID) const;

	FORCEINLINE void SetCurrentTwoHandedPosture(UAnimSequence* TwoHandedPosture) { CurrentTwoHandedPosture = TwoHandedPosture; }

	/*********************************
	 *  Bag manage
	 *********************************/
	void InitBags();

	/*********************************
	 *  Items manage
	 *********************************/
	bool TryAddItemToFirstFreeSlot(const FName& ItemID, const int32 Quantity);
	void TryAddItemToStack(const FName& ItemID, const int32 Quantity, const int32 MaxStack);
	
	/*********************************
	 *  Equipping manage
	 *********************************/
	AActor* SpawnItemActor(const FEquippingFragment& EquippingFragment, const EEquipSlot EItemSlot, AActor* Owner) const;
	
	void UseNewTwoHandedPosture(const FName& ItemID);
	void ResetCurrentTwoHandedPosture();

	/*********************************
	 *  Abilities and stats managing
	 *********************************/
	void ApplyItemStats(const FEquippingFragment& EquippingFragment) const;
	void RemoveItemStats(const FName& ItemID) const;
	void GiveWeaponAbilities(const FWeaponFragment* WeaponFragment, const EWeaponMode WeaponMode, const bool bAddForMainHand, const bool bAddForAuxiliaryHand);
	void RemoveWeaponAbilities(const EWeaponMode WeaponMode);
	void UpdateWeaponMode();

	
	/***********************************
	 *  Broadcast to widget controller
	 ***********************************/
	UFUNCTION(Client, Reliable)
	void BroadcastBagChange(const FBagData& BagData);
	UFUNCTION(Client, Reliable)
	void BroadcastBagRemoved(const FBagData& BagData);
	UFUNCTION(Client, Reliable)
	void BroadcastSlotChange(const int32 BagIndex, const int32 SlotIndex, const FInventorySlotData& ItemData);
	UFUNCTION(Client, Reliable)
	void BroadcastSlotRemove(const int32 BagIndex, const int32 SlotIndex, const FInventorySlotData& ItemData);
	UFUNCTION(Client, Reliable)
	void BroadcastEquipSlotChange(const FEquipSlotData& EquipSlotData);
	UFUNCTION(Client, Reliable)
	void BroadcastEquipSlotRemoved(const FEquipSlotData& EquipSlotData);
	
	/******************
	 *  Debug functions
	 ******************/
	void PrintMessage(const FString& InText) const;

private:

	UPROPERTY(Replicated)
	FBagList BagList;
	
	UPROPERTY(Replicated)
	FEquippedItemsList EquippedSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TMap<FGameplayTag, EEquipSlot> AssociatedInputTagWithSlot;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Table")
	TArray<FName> BagsDefaultID;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="Items Data Table")
	TSoftObjectPtr<UDataTable> Entries;

	UPROPERTY(Replicated)
	TObjectPtr<UAnimSequence> CurrentTwoHandedPosture = nullptr;

	TMap<EWeaponMode, FGrantedAbilityHandles> GrantedAbilityHandlesByMode;
	TMap<EEquipSlot, FGrantedAbilityHandles> GrantedEffectHandlesByInstance;

	UPROPERTY(Replicated)
	bool bIsTwoHandedEquip = false;
};
