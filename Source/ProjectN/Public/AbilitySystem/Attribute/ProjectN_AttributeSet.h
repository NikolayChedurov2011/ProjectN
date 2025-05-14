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
	 *  The strength is influence on, damage (*0,07), and passive on critical hit damage (*1,3), block chance (*0,0065), //poise (*0,15)
	 *************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Strength", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Strength)

	/*************************************
	*  The intelligence is influence on magical damage (*1,2), and passive on magical armor(*0,07), max mana (*2), mana regeneration (*0,05)
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Intelligence", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Intelligence)

	/*************************************
	*  The dexterity is passive influence on critical hit chance (*0,03), critical hit damage (*0,9), evasion (*0,06), armor penetration (*0,4)
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Dexterity", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Dexterity)

	/*************************************
	*  The vitality is passive influence on max health (*0,7), health regeneration (*0,05), //carrying capacity (*0,7)
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

	/*UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Stamina)
	
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxStamina)*/

	/*UPROPERTY(BlueprintReadOnly, Category = "Poise", ReplicatedUsing = OnRep_Poise)
	FGameplayAttributeData Poise;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Poise)
	
	UPROPERTY(BlueprintReadOnly, Category = "Poise", ReplicatedUsing = OnRep_MaxPoise)
	FGameplayAttributeData MaxPoise;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxPoise)*/
	
	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxMovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "EquipmentWeight", ReplicatedUsing = OnRep_EquipmentWeight)
	FGameplayAttributeData EquipmentWeight;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, EquipmentWeight)
	
	UPROPERTY(BlueprintReadOnly, Category = "CarryingCapacity", ReplicatedUsing = OnRep_MaxCarryingCapacity)
	FGameplayAttributeData MaxCarryingCapacity;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxCarryingCapacity)

	// Secondary
	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "MagicalArmor", ReplicatedUsing = OnRep_MagicalArmor)
	FGameplayAttributeData MagicalArmor;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MagicalArmor)
	
	/*UPROPERTY(BlueprintReadOnly, Category = "Evasion", ReplicatedUsing = OnRep_Evasion)
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Evasion)*/
	
	UPROPERTY(BlueprintReadOnly, Category = "BlockChance", ReplicatedUsing = OnRep_BlockChance)
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, BlockChance)
	
	UPROPERTY(BlueprintReadOnly, Category = "ArmorPenetration", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, ArmorPenetration)
	
	UPROPERTY(BlueprintReadOnly, Category = "CriticalHitChance", ReplicatedUsing = OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, CriticalHitChance)
	
	UPROPERTY(BlueprintReadOnly, Category = "CriticalHitDamage", ReplicatedUsing = OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, CriticalHitDamage)
	
	UPROPERTY(BlueprintReadOnly, Category = "HealRegeneration", ReplicatedUsing = OnRep_HealRegeneration)
	FGameplayAttributeData HealRegeneration;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, HealRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "ManaRegeneration", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, ManaRegeneration)

	/*************************************
	*  Meta attributes
	*************************************/
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, IncomingDamage)
	
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
	/*UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);*/
	/*UFUNCTION()
	virtual void OnRep_Poise(const FGameplayAttributeData& OldPoise);
	UFUNCTION()
	virtual void OnRep_MaxPoise(const FGameplayAttributeData& OldMaxPoise);*/
	UFUNCTION()
	virtual void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed);
	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

	UFUNCTION()
	virtual void OnRep_EquipmentWeight(const FGameplayAttributeData& OldEquipmentWeight);
	UFUNCTION()
	virtual void OnRep_MaxCarryingCapacity(const FGameplayAttributeData& OldMaxCarryingCapacity);
	
	// Secondary
	UFUNCTION()
    virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION()
    virtual void OnRep_MagicalArmor(const FGameplayAttributeData& OldMagicalArmor);
	/*UFUNCTION()
	virtual void OnRep_Evasion(const FGameplayAttributeData& OldEvasion);*/
	UFUNCTION()
	virtual void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance);
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
	void ShowFloatingText(const FEffectProperties& Props, const float Damage) const;
};
