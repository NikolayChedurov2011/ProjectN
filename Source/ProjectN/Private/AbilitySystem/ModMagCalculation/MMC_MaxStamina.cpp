// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxStamina.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	VitalityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetVitalityAttribute();
	VitalityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityCaptureDefinition.bSnapshot = false;

	DexterityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetDexterityAttribute();
	DexterityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DexterityCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityCaptureDefinition);
	RelevantAttributesToCapture.Add(DexterityCaptureDefinition);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float VitalityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(VitalityCaptureDefinition, Spec, EvaluateParameters, VitalityMagnitude);

	float DexterityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(DexterityCaptureDefinition, Spec, EvaluateParameters, DexterityMagnitude);
	
	const float Magnitude = DexterityMagnitude * 3 + VitalityMagnitude * 3 + 200.0f;

	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 Level = CombatInterface->GetCharacterLevel();
	
	return Magnitude;
}
