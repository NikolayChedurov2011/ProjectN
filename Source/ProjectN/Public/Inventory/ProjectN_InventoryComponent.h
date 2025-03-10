// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "Inventory/PojectN_InventoryItemsRecord.h"
#include "ProjectN_InventoryComponent.generated.h"

class AProjectN_CharacterBase;

USTRUCT()
struct FEquippedItemData
{
	GENERATED_BODY()

public:

	FEquippedItemData(){}
	FEquippedItemData(UProjectN_ItemInstance* InItemInstance, const EItemSlot InItemSlot, const TArray<FGameplayAbilitySpecHandle>& InGameplayAbilitySpecHandles, const TArray<FActiveGameplayEffectHandle>& InActiveGameplayEffectHandles)
	: ItemInstance(InItemInstance), ItemSlot(InItemSlot), GameplayAbilitySpecHandles(InGameplayAbilitySpecHandles), ActiveGameplayEffectHandles(InActiveGameplayEffectHandles) {}

	UPROPERTY()
	TObjectPtr<UProjectN_ItemInstance> ItemInstance = nullptr;

	UPROPERTY()
	EItemSlot ItemSlot = EItemSlot::None;
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GameplayAbilitySpecHandles;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void EquipTestItem();
	
	UProjectN_InventoryComponent();
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void AddItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void AddItemByInstance(UProjectN_ItemInstance* InItemInstance);
	UFUNCTION(BlueprintCallable)
	void RemoveItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void RemoveItemByInstance(UProjectN_ItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	void EquipItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void EquipItemByInstance(UProjectN_ItemInstance* InItemInstance);
	UFUNCTION(BlueprintCallable)
	void UnEquipItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void UnEquipItemByInstance(UProjectN_ItemInstance* InItemInstance);
	UFUNCTION(BlueprintCallable)
	void DropItem(UProjectN_ItemInstance* InItemInstance);
	UFUNCTION(BlueprintCallable)
	void ApplyItemAbilityAndEffects(const AProjectN_CharacterBase* BaseCharacter, UProjectN_ItemInstance* ItemInstance);
	void RemoveItemAbilityAndEffects(const FEquippedItemData& ItemData);

	UFUNCTION(BlueprintCallable)
	bool IsEquippableItem(UProjectN_ItemInstance* InItemInstance) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UProjectN_ItemInstance* GetEquippedItem()const { return CurrentItemInstance; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE  TArray<FInventoryItem>& GetItemsList() { return InventoryList.GetItemsRef(); }

	virtual void GameplayEventCallback(const FGameplayEventData* Payload);

	// This implementation in case if inventory classes should be independent, for example use it as separate plugin
	//static FGameplayTag EquipItemTag;
	//static FGameplayTag UnEquipItemTag;
	//static FGameplayTag DropItemTag;

protected:
	virtual void InitializeComponent() override;

	UFUNCTION()
	void AddInventoryTags();

	void HandleGameplayEventInternal(const FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(const FGameplayEventData Payload);

	UPROPERTY(Replicated, EditDefaultsOnly)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticClass>> DefaultItems;

	UPROPERTY(Replicated)
	TObjectPtr<UProjectN_ItemInstance> CurrentItemInstance = nullptr;

	UPROPERTY(Replicated)
	TArray<FEquippedItemData> EquippedItemsData;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
