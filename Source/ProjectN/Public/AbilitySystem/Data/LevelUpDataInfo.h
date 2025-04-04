// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpDataInfo.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 XPForLevelUp = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointsReward = 0;
};

UCLASS()
class PROJECTN_API ULevelUpDataInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FLevelUpInfo> LevelUpInformationContainer;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetLevelByXP(const int32 XP);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetXPForNextLevelUpByLevel(const int32 Level);
};
