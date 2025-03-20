// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "Inventory/PojectN_InventoryItemsRecord.h"
#include "ProjectN_InventoryComponent.generated.h"

class UProjectN_LootDataAsset;

USTRUCT()
struct FEquippedItemData
{
	GENERATED_BODY()

public:

	FEquippedItemData(){}
	FEquippedItemData(UProjectN_ItemInstance* InItemInstance, const EItemSlot InItemSlot) : ItemInstance(InItemInstance), ItemSlot(InItemSlot) {}

	UPROPERTY()
	UProjectN_ItemInstance* ItemInstance = nullptr;

	UPROPERTY()
	EItemSlot ItemSlot = EItemSlot::None;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable)
	//void EquipTestItem();
	
	UProjectN_InventoryComponent();
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void AddItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass, const int32 ItemStack);
	UFUNCTION(BlueprintCallable)
	void AddItemByInstance(UProjectN_ItemInstance* InItemInstance);
	//UFUNCTION(BlueprintCallable)
	//void RemoveItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void RemoveItemByInstance(UProjectN_ItemInstance* InItemInstance);

	//UFUNCTION(BlueprintCallable)
	//void EquipItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void EquipItemByInstance(UProjectN_ItemInstance* InItemInstance, const EItemSlot InSlot);
	//UFUNCTION(BlueprintCallable)
	//void UnEquipItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void UnEquipItemByInstance(UProjectN_ItemInstance* InItemInstance);
	UFUNCTION(BlueprintCallable)
	void DropItem(UProjectN_ItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	bool IsEquippableItem(UProjectN_ItemInstance* InItemInstance) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UProjectN_ItemInstance* GetEquippedItem()const { return CurrentItemInstance; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE  TArray<FInventoryItem>& GetItemsList() { return InventoryList.GetItemsRef(); }

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<FInventoryItem>& GetItems() { return InventoryList.GetItemsRef(); }

	FVector FindSocketLocationByTag(const FGameplayTag& InputTag);
	
protected:
	virtual void InitializeComponent() override;

	UFUNCTION()
	void AddInventoryTags();

	void HandleGameplayEventInternal(const FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(const FGameplayEventData Payload);
	
	/****
	 *  Slots managing
	 ****/
	bool IsSlotEquipped(const EItemSlot InItemSlot);
	bool IsSlotEquipped(const UProjectN_ItemInstance* InItemInstance);
	FEquippedItemData* FindItemDataBySlot(const EItemSlot InItemSlot);
	FEquippedItemData* FindItemDataByInstance(const UProjectN_ItemInstance* InItemInstance);
	void RemoveSlot(const EItemSlot InItemSlot);
	void AddItemToSlot(const EItemSlot InItemSlot, UProjectN_ItemInstance* InItemInstance);

	/****
	 *  Debug functions
	 ****/
	void PrintMessage(const FString& InText);

	UPROPERTY(Replicated, EditDefaultsOnly)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticClass>> DefaultItems;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UProjectN_ItemInstance>> DefaultItemInstance;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectN_LootDataAsset> DefaultLootData;

	UPROPERTY(Replicated)
	TObjectPtr<UProjectN_ItemInstance> CurrentItemInstance = nullptr;

	UPROPERTY(Replicated)
	TArray<FEquippedItemData> EquippedItemSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, EItemSlot> AssociatedTagWithSlot;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
