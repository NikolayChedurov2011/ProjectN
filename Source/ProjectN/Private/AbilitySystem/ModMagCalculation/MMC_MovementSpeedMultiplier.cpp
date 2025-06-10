// N Chedurov All Rights Reserved

#include "AbilitySystem/ModMagCalculation/MMC_MovementSpeedMultiplier.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

UMMC_MovementSpeedMultiplier::UMMC_MovementSpeedMultiplier()
{
	EquipmentWeightCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetEquipmentWeightAttribute();
	EquipmentWeightCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EquipmentWeightCaptureDefinition.bSnapshot = false;

	MaxCarryingCapacityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetMaxCarryingCapacityAttribute();
	MaxCarryingCapacityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxCarryingCapacityCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(EquipmentWeightCaptureDefinition);
	RelevantAttributesToCapture.Add(MaxCarryingCapacityCaptureDefinition);
}

float UMMC_MovementSpeedMultiplier::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float EquipmentWeightMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(EquipmentWeightCaptureDefinition, Spec, EvaluateParameters, EquipmentWeightMagnitude);

	float MaxCarryingCapacityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(MaxCarryingCapacityCaptureDefinition, Spec, EvaluateParameters, MaxCarryingCapacityMagnitude);

	EquipmentWeightMagnitude = EquipmentWeightMagnitude > MaxCarryingCapacityMagnitude? MaxCarryingCapacityMagnitude : EquipmentWeightMagnitude;
	
	const float CarryingCapacityPercent = EquipmentWeightMagnitude / MaxCarryingCapacityMagnitude;

	const float Magnitude = CarryingCapacityPercent > 0.55? FMath::Max(1 - CarryingCapacityPercent, 100) : 1;
	
	return Magnitude;
}
