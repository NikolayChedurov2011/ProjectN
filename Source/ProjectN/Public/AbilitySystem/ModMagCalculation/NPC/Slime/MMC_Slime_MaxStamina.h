// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Slime_MaxStamina.generated.h"

UCLASS()
class PROJECTN_API UMMC_Slime_MaxStamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UMMC_Slime_MaxStamina();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
protected:

	FGameplayEffectAttributeCaptureDefinition DexterityCaptureDefinition;
};