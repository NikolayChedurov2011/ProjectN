// N Chedurov All Rights Reserved


#include "AbilitySystem/ExecCalc/ProjectN_ExecCalc_Damage_Magical.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemLibrary.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

struct ProjectNMagicalDamageStatics
{
	// Source attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	// Target attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Evasion);
	
	ProjectNMagicalDamageStatics()
	{
		// Source attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Intelligence, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, CriticalHitDamage, Source, false);

		// Target attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, MagicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectN_AttributeSet, Evasion, Target, false);
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
	RelevantAttributesToCapture.Add(MagicalDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(MagicalDamageStatics().CriticalHitDamageDef);

	
	RelevantAttributesToCapture.Add(MagicalDamageStatics().MagicalArmorDef);
	RelevantAttributesToCapture.Add(MagicalDamageStatics().EvasionDef);
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

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Get damage set by caller magnitude
	float Damage = 0.f; /*Spec.GetSetByCallerMagnitude(ProjectNGameplayTags::Attribute_Meta_Damage, false);*/

	for(const FGameplayTag& Tag : FProjectNGameplayTagsStruct::Get().DamageTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Tag, false);
		Damage += DamageTypeValue;

		//TODO: Here we can apply modifiers or defense for each damage type
	}

	// Capture attributes
	/*********************
	 * Source attributes
	 *********************/
	float CapturedSourceIntelligence = 0.f;
	GetAttributeValue(ExecutionParams, MagicalDamageStatics().IntelligenceDef, EvaluateParameters, CapturedSourceIntelligence);

	float CapturedSourceCriticalHitChance = 0.f;
	GetAttributeValue(ExecutionParams, MagicalDamageStatics().CriticalHitChanceDef, EvaluateParameters, CapturedSourceCriticalHitChance);
	
	float CapturedSourceCriticalHitDamage = 0.f;
	GetAttributeValue(ExecutionParams, MagicalDamageStatics().CriticalHitDamageDef, EvaluateParameters, CapturedSourceCriticalHitDamage);
	
	/*********************
	 * Target attributes
	 *********************/
	float CapturedTargetMagicalArmor = 0.f;
	GetAttributeValue(ExecutionParams, MagicalDamageStatics().MagicalArmorDef, EvaluateParameters, CapturedTargetMagicalArmor);

	float CapturedTargetEvasionChance = 0.f;
	GetAttributeValue(ExecutionParams, MagicalDamageStatics().EvasionDef, EvaluateParameters, CapturedTargetEvasionChance);

	/*********************
	* Main calculations
	*********************/
	const bool bEvaded = FMath::RandRange(1, 100) < CapturedTargetEvasionChance;
	if (bEvaded)
	{
		UProjectN_AbilitySystemLibrary::SetIsEvaded(EffectContextHandle, bEvaded);
		
		// Result
		const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, 0);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);

		return;
	}

	// Add coefficient from Strength
	Damage += CapturedSourceIntelligence * 1.2f;
	const float EffectiveMagicArmor = CapturedTargetMagicalArmor *= (100 - (CapturedSourceIntelligence * 0.5f) * 0.25f) / 100.f;
	Damage *= (100 - EffectiveMagicArmor * 2.f) / 100.f;

	// Critical hit chance
	const bool bCritical = FMath::RandRange(1, 100) < CapturedSourceCriticalHitChance;
	if (bCritical)
	{
		UProjectN_AbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCritical);
		const float CriticalHitDamage = Damage * (CapturedSourceCriticalHitDamage / 100.f);
		Damage += CriticalHitDamage;
	}

	// Result
	const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UProjectN_ExecCalc_Damage_Magical::GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef, const FAggregatorEvaluateParameters& InEvalParams, float& OutMagnitude) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InCaptureDef, InEvalParams, OutMagnitude);
	OutMagnitude = FMath::Max<float>(OutMagnitude, 0.0f);
}