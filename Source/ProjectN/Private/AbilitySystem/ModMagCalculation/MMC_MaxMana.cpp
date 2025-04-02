// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxMana.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/CombatInterface.h"

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
	const float Magnitude = IntelligenceMagnitude * 10 + 200.0f;

	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 Level = CombatInterface->GetCharacterLevel();
	
	return Magnitude;
}
