// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ProjectN_ExecCalc_Damage_Magical.generated.h"

UCLASS()
class PROJECTN_API UProjectN_ExecCalc_Damage_Magical : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UProjectN_ExecCalc_Damage_Magical();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	void GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef, const FAggregatorEvaluateParameters& InEvalParams, float& OutMagnitude) const;
};
