// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/NPC/Slime/MMC_Slime_MaxHealth.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/NPCInterface.h"

UMMC_Slime_MaxHealth::UMMC_Slime_MaxHealth()
{
	VitalityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetVitalityAttribute();
	VitalityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityCaptureDefinition);
}

float UMMC_Slime_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float VitalityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(VitalityCaptureDefinition, Spec, EvaluateParameters, VitalityMagnitude);

	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UNPCInterface>())
	{
		Level = INPCInterface::Execute_GetNPCLevel(Spec.GetContext().GetSourceObject());
	}
	
	const float Magnitude = VitalityMagnitude * 1.4f + Level * 53.0f;
	return Magnitude;
}
