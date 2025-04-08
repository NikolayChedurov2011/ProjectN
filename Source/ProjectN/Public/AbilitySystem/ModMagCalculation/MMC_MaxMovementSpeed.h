// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMovementSpeed.generated.h"

UCLASS()
class PROJECTN_API UMMC_MaxMovementSpeed : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MaxMovementSpeed();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:

	FGameplayEffectAttributeCaptureDefinition EquipmentWeightCaptureDefinition;
	FGameplayEffectAttributeCaptureDefinition MaxCarryingCapacityCaptureDefinition;
	FGameplayEffectAttributeCaptureDefinition MaxMovementSpeedCaptureDefinition;
};
