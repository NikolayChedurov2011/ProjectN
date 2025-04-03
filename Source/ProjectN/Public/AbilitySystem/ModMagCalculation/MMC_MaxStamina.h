// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxStamina.generated.h"

UCLASS()
class PROJECTN_API UMMC_MaxStamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_MaxStamina();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:

	FGameplayEffectAttributeCaptureDefinition VitalityCaptureDefinition;
	FGameplayEffectAttributeCaptureDefinition DexterityCaptureDefinition;
};