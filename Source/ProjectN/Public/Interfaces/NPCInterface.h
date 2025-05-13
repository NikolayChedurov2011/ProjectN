// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ProjectN_NPCDataAsset.h"
#include "UObject/Interface.h"
#include "NPCInterface.generated.h"

enum class ENPCRarity : uint8;

UINTERFACE(MinimalAPI, BlueprintType)
class UNPCInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTN_API INPCInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetNPCRewardXP() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ENPCRarity GetNPCRarity() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetNPCLevel() const;
};
