// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/ActorComponent.h"
#include "Inventory/PojectN_InventoryItemsRecord.h"
#include "ProjectN_InventoryComponent.generated.h"


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

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
