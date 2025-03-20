// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_LootDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FItemLootData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Loot Data")
	TSubclassOf<UItemStaticClass> ItemStaticClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Loot Data", meta=(ClampMin = 1))
	int32 MinCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Loot Data", meta=(ClampMin = 1))
	int32 MaxCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Loot Data", meta=(ClampMin = 0.1f), meta=(ClampMax = 100.f))
	float DropChance = 1;
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_LootDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemLootData> LootData;
};
