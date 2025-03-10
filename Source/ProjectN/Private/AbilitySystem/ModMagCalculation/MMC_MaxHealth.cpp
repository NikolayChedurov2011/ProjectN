// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxHealth.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	CaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetVitalityAttribute();
	CaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CaptureDefinition.bSnapshot = false;
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Magnitude = 0.0f;
	GetCapturedAttributeMagnitude(CaptureDefinition, Spec, EvaluateParameters, Magnitude);
	Magnitude = FMath::Max(Magnitude, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 Level = CombatInterface->GetCharacterLevel();
	
	return Super::CalculateBaseMagnitude_Implementation(Spec);
}
