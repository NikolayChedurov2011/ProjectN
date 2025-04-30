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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
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
	AProjectN_ItemActor_Base* GetEquippedWeaponBySlot(const EItemSlot InItemSlot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetWeaponTypeBySlot(const EItemSlot InItemSlot);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE  TArray<FInventoryItem>& GetItemsList() { return InventoryList.GetItemsRef(); }

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FInventoryItem>& GetItems() { return InventoryList.GetItemsRef(); }

	FVector FindSocketLocationBySlot(const EItemSlot ItemSlot);

	/******************************
	 *   For inventory component
	 ******************************/
	UFUNCTION(Client, Reliable)
	void ClientUpdateItemInfo(UProjectN_ItemInstance* ItemInstance);
	
	UFUNCTION(Client, Reliable)
	void ClientRemoveItem(UProjectN_ItemInstance* ItemInstance);
	
protected:
	virtual void InitializeComponent() override;

	UFUNCTION()
	void AddInventoryTags();

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

private:
	
	TMap<EWeaponMode, FGrantedAbilityHandles> GrantedAbilityHandlesByMode;
	TMap<EItemSlot, FGrantedAbilityHandles> GrantedEffectHandlesByInstance;
};
