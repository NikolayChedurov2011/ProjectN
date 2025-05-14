// N Chedurov All Rights Reserved


#include "AbilitySystem/ExecCalc/ProjectN_ExecCalc_Damage_Physical.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

struct ProjectNDamageStatics
{
	// Source attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	// Target attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Evasion);
	
	ProjectNDamageStatics()
	{
		// Source attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, CriticalHitDamage, Source, false);

		// Target attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, BlockChance, Target, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Evasion, Target, false);
	}
};

static const ProjectNDamageStatics& DamageStatics()
{
	static ProjectNDamageStatics DStatics;
	return DStatics;
}

UProjectN_ExecCalc_Damage_Physical::UProjectN_ExecCalc_Damage_Physical()
{
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UProjectN_ExecCalc_Damage_Physical::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	/**********************************
	 * Get damage set by caller magnitude
	 **********************************/
	float Damage = Spec.GetSetByCallerMagnitude(ProjectNGameplayTags::Attribute_Meta_Damage, false);

	// Capture attributes
	/*********************
	 * Source attributes
	 *********************/
	float CapturedSourceStrength = 0.f;
	GetAttributeValue(ExecutionParams, DamageStatics().StrengthDef, EvaluateParameters, CapturedSourceStrength);
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluateParameters, CapturedSourceStrength);
	//CapturedSourceStrength = FMath::Max<float>(CapturedSourceStrength, 0.0f);

	float CapturedSourceArmorPenetration = 0.f;
	GetAttributeValue(ExecutionParams, DamageStatics().ArmorPenetrationDef, EvaluateParameters, CapturedSourceArmorPenetration);
	
	float CapturedSourceCriticalHitChance = 0.f;
	GetAttributeValue(ExecutionParams, DamageStatics().CriticalHitChanceDef, EvaluateParameters, CapturedSourceCriticalHitChance);
	
	float CapturedSourceCriticalHitDamage = 0.f;
	GetAttributeValue(ExecutionParams, DamageStatics().CriticalHitDamageDef, EvaluateParameters, CapturedSourceCriticalHitDamage);
	
	/*********************
	 * Target attributes
	 *********************/
	float CapturedTargetArmor = 0.f;
	GetAttributeValue(ExecutionParams, DamageStatics().ArmorDef, EvaluateParameters, CapturedTargetArmor);

	float CapturedTargetBlockChance = 0.f;
	GetAttributeValue(ExecutionParams, DamageStatics().BlockChanceDef, EvaluateParameters, CapturedTargetBlockChance);

	/*********************
	* Main calculations
	*********************/
	const bool bBlocked = FMath::RandRange(1, 100) < CapturedTargetBlockChance;

	if (bBlocked)
	{
		// Result
		const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, 0);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);

		return;
	}

	// Add coefficient from Strength
	Damage += CapturedSourceStrength * 0.07f;
	const float EffectiveArmor = CapturedTargetArmor *= (100 - CapturedSourceArmorPenetration * 0.25f) / 100.f;
	Damage *= (100 - EffectiveArmor * 2.f) / 100.f;

	
	// Result
	const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UProjectN_ExecCalc_Damage_Physical::GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef, const FAggregatorEvaluateParameters& InEvalParams, float& OutMagnitude) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InCaptureDef, InEvalParams, OutMagnitude);
	OutMagnitude = FMath::Max<float>(OutMagnitude, 0.0f);
}