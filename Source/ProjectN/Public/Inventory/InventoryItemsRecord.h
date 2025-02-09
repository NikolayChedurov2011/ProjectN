// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryItemsRecord.generated.h"

// List of items
USTRUCT(BlueprintType)
struct FInventoryItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UItemInstance* ItemInstance = nullptr;
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

	void AddItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	void RemoveItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass);
	TArray<FInventoryItem>& GetItemsRef() { return Items; }

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
