// N Chedurov All Rights Reserved


#include "AbilitySystem/ExecCalc/ProjectN_ExecCalc_Damage_Physical.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemLibrary.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "ProjectN/ProjectNTypes.h"

struct ProjectNPhysicalDamageStatics
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
	
	ProjectNPhysicalDamageStatics()
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

static const ProjectNPhysicalDamageStatics& PhysicalDamageStatics()
{
	static ProjectNPhysicalDamageStatics PhysicalDStatics;
	return PhysicalDStatics;
}

UProjectN_ExecCalc_Damage_Physical::UProjectN_ExecCalc_Damage_Physical()
{
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().BlockChanceDef);
}

void UProjectN_ExecCalc_Damage_Physical::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	/**********************************
	 * Get damage set by caller magnitude
	 **********************************/
	float Damage = 0.f; /*Spec.GetSetByCallerMagnitude(ProjectNGameplayTags::Attribute_Meta_Damage, false);*/

	for(const FGameplayTag& Tag : FProjectNGameplayTagsStruct::Get().DamageTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Tag, false);
		Damage += DamageTypeValue;

		//TODO: Here we can apply modifiers or defence for each damage type
	}

	// Capture attributes
	/*********************
	 * Source attributes
	 *********************/
	float CapturedSourceStrength = 0.f;
	GetAttributeValue(ExecutionParams, PhysicalDamageStatics().StrengthDef, EvaluateParameters, CapturedSourceStrength);

	float CapturedSourceArmorPenetration = 0.f;
	GetAttributeValue(ExecutionParams, PhysicalDamageStatics().ArmorPenetrationDef, EvaluateParameters, CapturedSourceArmorPenetration);
	
	float CapturedSourceCriticalHitChance = 0.f;
	GetAttributeValue(ExecutionParams, PhysicalDamageStatics().CriticalHitChanceDef, EvaluateParameters, CapturedSourceCriticalHitChance);
	
	float CapturedSourceCriticalHitDamage = 0.f;
	GetAttributeValue(ExecutionParams, PhysicalDamageStatics().CriticalHitDamageDef, EvaluateParameters, CapturedSourceCriticalHitDamage);
	
	/*********************
	 * Target attributes
	 *********************/
	float CapturedTargetArmor = 0.f;
	GetAttributeValue(ExecutionParams, PhysicalDamageStatics().ArmorDef, EvaluateParameters, CapturedTargetArmor);

	float CapturedTargetBlockChance = 0.f;
	GetAttributeValue(ExecutionParams, PhysicalDamageStatics().BlockChanceDef, EvaluateParameters, CapturedTargetBlockChance);

	/*********************
	* Main calculations
	*********************/
	const bool bBlocked = FMath::RandRange(1, 100) < CapturedTargetBlockChance;
	if (bBlocked)
	{
		UProjectN_AbilitySystemLibrary::SetIsBlock(EffectContextHandle, bBlocked);
		
		// Result
		const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, 0);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);

		return;
	}

	// Add coefficient from Strength
	Damage += CapturedSourceStrength * 0.07f;
	const float EffectiveArmor = CapturedTargetArmor *= (100 - CapturedSourceArmorPenetration * 0.25f) / 100.f;
	Damage *= (100 - EffectiveArmor * 2.f) / 100.f;

	// Critical hit chance
	const bool bCritical = FMath::RandRange(1, 100) < CapturedSourceCriticalHitChance;
	if (bCritical)
	{
		UProjectN_AbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCritical);
		const float CriticalHitDamage = Damage * (CapturedSourceCriticalHitDamage / 100.f);
		Damage += CriticalHitDamage;
	}
	
	// Result
	const FGameplayModifierEvaluatedData EvaluatedData(UProjectN_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UProjectN_ExecCalc_Damage_Physical::GetAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef, const FAggregatorEvaluateParameters& InEvalParams, float& OutMagnitude) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InCaptureDef, InEvalParams, OutMagnitude);
	OutMagnitude = FMath::Max<float>(OutMagnitude, 0.0f);
}