// N Chedurov All Rights Reserved


#include "AbilitySystem/ExecCalc/ProjectN_ExecCalc_Damage_Magical.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

struct ProjectNMagicalDamageStatics
{
	// Source attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);

	// Target attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmor);
	
	ProjectNMagicalDamageStatics()
	{
		// Source attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Intelligence, Source, false);

		// Target attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Intelligence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, MagicalArmor, Target, false);
	}
};

static const ProjectNMagicalDamageStatics& MagicalDamageStatics()
{
	static ProjectNMagicalDamageStatics MagicalDStatics;
	return MagicalDStatics;
}

UProjectN_ExecCalc_Damage_Magical::UProjectN_ExecCalc_Damage_Magical()
{
	RelevantAttributesToCapture.Add(MagicalDamageStatics().IntelligenceDef);
}

void UProjectN_ExecCalc_Damage_Magical::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar  = SourceASC? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar  = TargetASC? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Get damage set by caller magnitude
	float Damage = Spec.GetSetByCallerMagnitude(ProjectNGameplayTags::Attribute_Meta_Damage, false);

	float CapturedStrength = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluateParameters, CapturedStrength);
	CapturedStrength = FMath::Max<float>(CapturedStrength, 0.0f);

	float CapturedArmor = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, CapturedArmor);
	CapturedArmor = FMath::Max<float>(CapturedArmor, 0.0f);


	// Result
	const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}