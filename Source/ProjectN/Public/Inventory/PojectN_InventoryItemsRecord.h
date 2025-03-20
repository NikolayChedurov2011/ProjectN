// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_ItemInstance.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PojectN_InventoryItemsRecord.generated.h"

// List of items
USTRUCT(BlueprintType)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	UProjectN_ItemInstance* ItemInstance = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryItem, FInventoryList>(Items, DeltaParams, *this);
	}

	void AddItemByStaticClass(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass, const int32 ItemStack);
	void AddItemByInstance(UProjectN_ItemInstance* InItemInstance, const int32 ItemStack);
	void RemoveItemByStaticClass(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass);
	void RemoveItemByInstance(const UProjectN_ItemInstance* InItemInstance);
	FInventoryItem* FindItemByInstance(const UProjectN_ItemInstance* InItemInstance);
	FInventoryItem* FindItemByClassWithInstance(const UProjectN_ItemInstance* InItemInstance);
	FInventoryItem* FindItemByClass(const UItemStaticClass* InItemClass);
	FORCEINLINE TArray<FInventoryItem>& GetItemsRef() { return Items; }

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
