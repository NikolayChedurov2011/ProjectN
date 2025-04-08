// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxMana.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/AvatarInfoInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetIntelligenceAttribute();
	IntelligenceCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceCaptureDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float IntelligenceMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceCaptureDefinition, Spec, EvaluateParameters, IntelligenceMagnitude);
	
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UAvatarInfoInterface>())
	{
		Level = IAvatarInfoInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	const float Magnitude = IntelligenceMagnitude * 2.f + Level * 20;
	
	return Magnitude;
}
