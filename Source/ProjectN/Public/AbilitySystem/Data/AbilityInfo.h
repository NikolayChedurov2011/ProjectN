// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectN/ProjectNTypes.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FProjectNActionSlotInfo2
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ActionSlotIndex = INDEX_NONE;
	
	UPROPERTY(BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	EEntryType EntryType = EEntryType::None;
};

UCLASS()
class PROJECTN_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FProjectNActionSlotInfo2> AbilityInformation;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FProjectNActionSlotInfo2& GetAbilityInfoByIndex(const int32 InActionSlotIndex);
};
