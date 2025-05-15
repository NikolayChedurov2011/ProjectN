// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_MaxMovementSpeed.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

UMMC_MaxMovementSpeed::UMMC_MaxMovementSpeed()
{
	EquipmentWeightCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetEquipmentWeightAttribute();
	EquipmentWeightCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EquipmentWeightCaptureDefinition.bSnapshot = false;

	MaxCarryingCapacityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetMaxCarryingCapacityAttribute();
	MaxCarryingCapacityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxCarryingCapacityCaptureDefinition.bSnapshot = false;

	MaxMovementSpeedCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetMaxMovementSpeedAttribute();
	MaxMovementSpeedCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxMovementSpeedCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(EquipmentWeightCaptureDefinition);
	RelevantAttributesToCapture.Add(MaxCarryingCapacityCaptureDefinition);
	RelevantAttributesToCapture.Add(MaxMovementSpeedCaptureDefinition);
}

float UMMC_MaxMovementSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	float MaxMovementSpeedMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(MaxMovementSpeedCaptureDefinition, Spec, EvaluateParameters, MaxMovementSpeedMagnitude);
	
	/*int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UAvatarInfoInterface>())
	{
		Level = IAvatarInfoInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}*/

	EquipmentWeightMagnitude = EquipmentWeightMagnitude > MaxCarryingCapacityMagnitude? MaxCarryingCapacityMagnitude : EquipmentWeightMagnitude;
	
	const float CarryingCapacityPercent = EquipmentWeightMagnitude / MaxCarryingCapacityMagnitude;

	const float Magnitude = CarryingCapacityPercent > 0.55? FMath::Max(MaxMovementSpeedMagnitude * (1 - CarryingCapacityPercent), 100) : MaxMovementSpeedMagnitude;
	
	return Magnitude;
}
