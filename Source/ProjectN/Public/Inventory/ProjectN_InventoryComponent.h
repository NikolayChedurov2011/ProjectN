// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
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
	void AddItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);

	UFUNCTION(BlueprintCallable)
	void EquipItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	UFUNCTION(BlueprintCallable)
	void UnEquipItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UProjectN_ItemInstance* GetEquippedItem()const { return CurrentItemInstance; }

protected:
	virtual void InitializeComponent() override;

	UPROPERTY(Replicated, EditDefaultsOnly)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticClass>> DefaultItems;

	UPROPERTY(Replicated)
	TObjectPtr<UProjectN_ItemInstance> CurrentItemInstance = nullptr;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
