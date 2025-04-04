// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ProjectN_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FPropertiesData
{
	GENERATED_BODY()

public:
	FPropertiesData() {}

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> AvatarActor = nullptr;
	
	UPROPERTY()
	TObjectPtr<AController> Controller = nullptr;
	
	UPROPERTY()
	TObjectPtr<ACharacter> Character = nullptr;
};

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

public:

	FGameplayEffectContextHandle EffectContextHandle;
	
	FPropertiesData SourceProperties;
	FPropertiesData TargetProperties;
};

//
// Using this template we can implement ptr to any static function with T return type
// For example TStaticFuncPtr<FGameplayAttribute()> can be equal to GetStrengthAttribute()
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;
//
//

UCLASS()
class PROJECTN_API UProjectN_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UProjectN_AttributeSet();

	//TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributeFunction;
	TMap<FGameplayTag, FGameplayAttribute> TagsToAttribute;

	/*
	 ***********************
	 *  Primary Attributes
	 ***********************
	 */

	/*************************************
	 *  The strength is influence on max health, damage, critical hit damage, armor penetration
	 *************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Strength", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Strength)

	/*************************************
	*  The intelligence is influence on magical damage, max mana, mana regeneration
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Intelligence", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Intelligence)

	/*************************************
	*  The dexterity is influence on critical hit chance, critical hit damage, evasion, armor, armor penetration, max poise, max stamina
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Dexterity", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Dexterity)

	/*************************************
	*  The vitality is influence on max health, max stamina, health regeneration, max poise
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Vitality", ReplicatedUsing = OnRep_Vitality)
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Vitality)

	
	/*
	 ************************
	 *  Secondary Attributes
	 ************************
	 */
	// Main
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Stamina)
	
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Poise", ReplicatedUsing = OnRep_Poise)
	FGameplayAttributeData Poise;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Poise)
	
	UPROPERTY(BlueprintReadOnly, Category = "Poise", ReplicatedUsing = OnRep_MaxPoise)
	FGameplayAttributeData MaxPoise;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxPoise)
	
	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxMovementSpeed)

	// Secondary
	/*************************************
	*  The armor is influence on defence and depends on dexterity
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Armor)
	
	/*************************************
	*  The evasion is influence on evasion chance and depends on dexterity
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Evasion", ReplicatedUsing = OnRep_Evasion)
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Evasion)

	/*************************************
	*  The armor penetration is influence on armor penetration and depends on dexterity and strength
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "ArmorPenetration", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, ArmorPenetration)
	
	/*************************************
	*  The critical hit chance is influence on critical hit chance and depends on dexterity
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "CriticalHitChance", ReplicatedUsing = OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, CriticalHitChance)
	
	/*************************************
	*  The critical hit damage is influence on critical hit damage and depends on strength + dexterity
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "CriticalHitDamage", ReplicatedUsing = OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, CriticalHitDamage)
	
	/*************************************
	*  The health regeneration is influence on health regeneration and depends on vitality
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "HealRegeneration", ReplicatedUsing = OnRep_HealRegeneration)
	FGameplayAttributeData HealRegeneration;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, HealRegeneration)
	
	/*************************************
	*  The mana regeneration is influence on mana regeneration and depends on intelligence
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "ManaRegeneration", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, ManaRegeneration)

	/*************************************
	*  Meta attributes
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, IncomingXP)

protected:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void SendXPEvent(const FEffectProperties& Props) const;

	// Primary Attributes
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);
	UFUNCTION()
	virtual void OnRep_Vitality(const FGameplayAttributeData& OldVitality);

	// Secondary Attributes
	// Main
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	UFUNCTION()
	virtual void OnRep_Poise(const FGameplayAttributeData& OldPoise);
	UFUNCTION()
	virtual void OnRep_MaxPoise(const FGameplayAttributeData& OldMaxPoise);
	UFUNCTION()
	virtual void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed);
	
	// Secondary
	UFUNCTION()
    virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION()
	virtual void OnRep_Evasion(const FGameplayAttributeData& OldEvasion);
	UFUNCTION()
	virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);
	UFUNCTION()
	virtual void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);
	UFUNCTION()
	virtual void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);
	UFUNCTION()
	virtual void OnRep_HealRegeneration(const FGameplayAttributeData& OldHealRegeneration);
	UFUNCTION()
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration);
	
private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
