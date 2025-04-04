// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/NPC/Slime/MMC_Slime_MaxStamina.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/NPCInterface.h"

UMMC_Slime_MaxStamina::UMMC_Slime_MaxStamina()
{
	DexterityCaptureDefinition.AttributeToCapture = UProjectN_AttributeSet::GetDexterityAttribute();
	DexterityCaptureDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DexterityCaptureDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(DexterityCaptureDefinition);
}

float UMMC_Slime_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float DexterityMagnitude = 0.0f;
	GetCapturedAttributeMagnitude(DexterityCaptureDefinition, Spec, EvaluateParameters, DexterityMagnitude);
	
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UNPCInterface>())
	{
		Level = INPCInterface::Execute_GetNPCLevel(Spec.GetContext().GetSourceObject());
	}
	
	const float Magnitude = DexterityMagnitude * 0.6f + Level * 30.0f;
	
	return Magnitude;
}
