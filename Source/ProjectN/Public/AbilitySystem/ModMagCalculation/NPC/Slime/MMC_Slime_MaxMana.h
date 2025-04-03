// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Slime_MaxMana.generated.h"

UCLASS()
class PROJECTN_API UMMC_Slime_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_Slime_MaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:

	FGameplayEffectAttributeCaptureDefinition IntelligenceCaptureDefinition;
};
