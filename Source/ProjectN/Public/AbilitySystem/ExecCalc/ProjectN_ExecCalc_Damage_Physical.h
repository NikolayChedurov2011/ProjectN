// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ProjectN_ExecCalc_Damage_Physical.generated.h"

UCLASS()
class PROJECTN_API UProjectN_ExecCalc_Damage_Physical : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UProjectN_ExecCalc_Damage_Physical();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:

	void GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef, const FAggregatorEvaluateParameters& InEvalParams, float& OutMagnitude) const;
};