// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTN_API IPlayerInterface
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddXP(const int32 XPToAdd);

	UFUNCTION(BlueprintNativeEvent)
	void AddToLevel(const int32 LevelsToAdd);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(const int32 AttributePointsToAdd);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 InLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetLevelByXP(const int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUP();
};
