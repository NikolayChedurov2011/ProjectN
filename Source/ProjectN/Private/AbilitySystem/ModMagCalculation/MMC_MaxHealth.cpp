// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxHealth.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetVitalityAttribute();
	VitalityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityCaptureDefinition.bSnapshot = false;

	StrengthCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetVitalityAttribute();
	StrengthCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityCaptureDefinition);
	RelevantAttributesToCapture.Add(StrengthCaptureDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float VitalityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(VitalityCaptureDefinition, Spec, EvaluateParameters, VitalityMagnitude);

	float StrengthMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(StrengthCaptureDefinition, Spec, EvaluateParameters, StrengthMagnitude);
	
	const float Magnitude = StrengthMagnitude +  VitalityMagnitude * 13 + 200.0f;

	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 Level = CombatInterface->GetCharacterLevel();
	
	return Magnitude;
}
