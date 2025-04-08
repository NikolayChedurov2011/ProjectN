// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxCarryingCapacity.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

UMMC_MaxCarryingCapacity::UMMC_MaxCarryingCapacity()
{
	VitalityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetVitalityAttribute();
	VitalityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityCaptureDefinition);
}

float UMMC_MaxCarryingCapacity::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float VitalityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(VitalityCaptureDefinition, Spec, EvaluateParameters, VitalityMagnitude);
	
	/*int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UAvatarInfoInterface>())
	{
		Level = IAvatarInfoInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}*/

	const float Magnitude = VitalityMagnitude * .7f + 10;
	
	return Magnitude;
}
