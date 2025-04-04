// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/NPC/Slime/MMC_Slime_MaxMana.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/NPCInterface.h"

UMMC_Slime_MaxMana::UMMC_Slime_MaxMana()
{
	IntelligenceCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetIntelligenceAttribute();
	IntelligenceCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceCaptureDefinition);
}

float UMMC_Slime_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float IntelligenceMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceCaptureDefinition, Spec, EvaluateParameters, IntelligenceMagnitude);
	
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UNPCInterface>())
	{
		Level = INPCInterface::Execute_GetNPCLevel(Spec.GetContext().GetSourceObject());
	}

	const float Magnitude = IntelligenceMagnitude * 1.2f + Level * 20.0f;
	return Magnitude;
}
