// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "ProjectN_NPCDataAsset.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ENPCRarity : uint8
{
	Common,
	Rare,
	Epic,
	Legendary
};


USTRUCT(BlueprintType)
struct FNPCDefaultData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="NPC Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="NPC Defaults")
	TArray<TSubclassOf<UGameplayAbility>> NPCAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Defaults")
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;
	
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat RewardXP = FScalableFloat();
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_NPCDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<ENPCRarity, FNPCDefaultData> NPCDataMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttributesDependency;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> InitializeAttributes;

	FNPCDefaultData GetNPCData(const ENPCRarity NPCRarity);
};
