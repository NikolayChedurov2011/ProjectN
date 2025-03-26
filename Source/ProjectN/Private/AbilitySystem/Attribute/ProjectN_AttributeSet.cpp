// N Chedurov All Rights Reserved


#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "ProjectN_GameplayTags.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UProjectN_AttributeSet::UProjectN_AttributeSet()
{
	// Add primary attributes to map
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Primary_Strength, GetStrengthAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Primary_Intelligence, GetIntelligenceAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Primary_Dexterity, GetDexterityAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Primary_Vitality, GetVitalityAttribute());

	// Add main attributes to map
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_Health, GetHealthAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_MaxHealth, GetMaxHealthAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_Mana, GetManaAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_MaxMana, GetMaxManaAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_Stamina, GetStaminaAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_MaxStamina, GetMaxStaminaAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_Poise, GetPoiseAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_MaxPoise, GetMaxPoiseAttribute());
	
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Main_MovementSpeed, GetMaxMovementSpeedAttribute());

	// Add secondary attributes to map
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_Armor, GetArmorAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_Evasion, GetEvasionAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_HealRegeneration, GetHealRegenerationAttribute());
	TagsToAttribute.Add(ProjectNGameplayTags::Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute());

	// Add progression attributes to map
	TagsToAttribute.Add(ProjectNGameplayTags::Progression_AttributePoint, GetAttributePointAttribute());


	
	// Add primary attributes to map
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Primary_Vitality, GetVitalityAttribute);

	// Add main attributes to map
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_Health, GetHealthAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_Mana, GetManaAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_MaxMana, GetMaxManaAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_Stamina, GetStaminaAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_MaxStamina, GetMaxStaminaAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_Poise, GetPoiseAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_MaxPoise, GetMaxPoiseAttribute);
	
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Main_MovementSpeed, GetMaxMovementSpeedAttribute);

	// Add secondary attributes to map
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_Evasion, GetEvasionAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_HealRegeneration, GetHealRegenerationAttribute);
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);

	// Add progression attributes to map
	TagsToAttributeFunction.Add(ProjectNGameplayTags::Progression_AttributePoint, GetAttributePointAttribute);
}

void UProjectN_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// Current attribute values
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if (Data.EvaluatedData.Attribute == GetPoiseAttribute())
	{
		SetPoise(FMath::Clamp(GetPoise(), 0.f, GetMaxPoise()));
	}

	// Max attribute values
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > Data.EvaluatedData.Magnitude)
		{
			SetHealth(Data.EvaluatedData.Magnitude);
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		if (GetMana() > Data.EvaluatedData.Magnitude)
		{
			SetMana(Data.EvaluatedData.Magnitude);
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		if (GetStamina() > Data.EvaluatedData.Magnitude)
		{
			SetStamina(Data.EvaluatedData.Magnitude);
		}
	}
	if (Data.EvaluatedData.Attribute == GetMaxPoiseAttribute())
	{
		if (GetPoise() > Data.EvaluatedData.Magnitude)
		{
			SetPoise(Data.EvaluatedData.Magnitude);
		}
	}
}

void UProjectN_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UProjectN_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Vitality, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	// Main
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Poise, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxPoise, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, MaxMovementSpeed, COND_None, REPNOTIFY_Always);

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, Evasion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, HealRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	// Progression
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectN_AttributeSet, AttributePoint, COND_None, REPNOTIFY_Always);
}

void UProjectN_AttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceProperties.AbilitySystemComponent = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceProperties.AbilitySystemComponent) && Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo.IsValid() && Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceProperties.AvatarActor = Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		Props.SourceProperties.Controller = Props.SourceProperties.AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceProperties.Controller == nullptr && Props.SourceProperties.AvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceProperties.AvatarActor))
			{
				Props.SourceProperties.Controller = Pawn->GetController();
			}
		}
		if (Props.SourceProperties.Controller)
		{
			if (ACharacter* SourceCharacter = Cast<ACharacter>(Props.SourceProperties.Controller->GetPawn()))
			{
				Props.SourceProperties.Character = SourceCharacter;
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetProperties.AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetProperties.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetProperties.Character = Cast<ACharacter>(Props.TargetProperties.AvatarActor);
		Props.TargetProperties.AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetProperties.AvatarActor);
	}
}

void UProjectN_AttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Strength, OldStrength);
}

void UProjectN_AttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Intelligence, OldIntelligence);
}

void UProjectN_AttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Dexterity, OldDexterity);
}

void UProjectN_AttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Vitality, OldVitality);
}

void UProjectN_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Health, OldHealth);
}

void UProjectN_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Health, OldMaxHealth);
}

void UProjectN_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Mana, OldMana);
}

void UProjectN_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxMana, OldMaxMana);
}

void UProjectN_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Stamina, OldStamina);
}

void UProjectN_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxStamina, OldMaxStamina);
}

void UProjectN_AttributeSet::OnRep_Poise(const FGameplayAttributeData& OldPoise)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Poise, OldPoise);
}

void UProjectN_AttributeSet::OnRep_MaxPoise(const FGameplayAttributeData& OldMaxPoise)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxPoise, OldMaxPoise);
}

void UProjectN_AttributeSet::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, MaxMovementSpeed, OldMaxMovementSpeed);
}

void UProjectN_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Armor, OldArmor);
}

void UProjectN_AttributeSet::OnRep_Evasion(const FGameplayAttributeData& OldEvasion)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, Evasion, OldEvasion);
}

void UProjectN_AttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UProjectN_AttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UProjectN_AttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UProjectN_AttributeSet::OnRep_HealRegeneration(const FGameplayAttributeData& OldHealRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, HealRegeneration, OldHealRegeneration);
}

void UProjectN_AttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, ManaRegeneration, OldManaRegeneration);
	
}void UProjectN_AttributeSet::OnRep_AttributePoint(const FGameplayAttributeData& OldAttributePoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectN_AttributeSet, AttributePoint, OldAttributePoint);
}