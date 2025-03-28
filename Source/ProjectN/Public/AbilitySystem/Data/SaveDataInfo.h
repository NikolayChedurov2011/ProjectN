// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SaveDataInfo.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FProjectNAttributeSaveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float AttributeValue = 0.f;
};

UCLASS()
class PROJECTN_API USaveDataInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString SavedName = FString();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FProjectNAttributeSaveInfo> AttributeInformation;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FProjectNAttributeSaveInfo& GetAttributeInfoByTag(const FGameplayTag& Tag);
};
