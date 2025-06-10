// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MovementSpeedMultiplier.generated.h"

UCLASS()
class PROJECTN_API UMMC_MovementSpeedMultiplier : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MovementSpeedMultiplier();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:

	FGameplayEffectAttributeCaptureDefinition EquipmentWeightCaptureDefinition;
	FGameplayEffectAttributeCaptureDefinition MaxCarryingCapacityCaptureDefinition;
};
