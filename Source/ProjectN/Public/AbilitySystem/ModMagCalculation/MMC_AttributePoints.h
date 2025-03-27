// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_AttributePoints.generated.h"

UCLASS()
class PROJECTN_API UMMC_AttributePoints : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
