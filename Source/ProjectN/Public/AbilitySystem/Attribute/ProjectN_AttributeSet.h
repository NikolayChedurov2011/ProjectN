// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
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

UCLASS()
class PROJECTN_API UProjectN_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UProjectN_AttributeSet();

	/*
	 ***********************
	 *  Primary Attributes
	 ***********************
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Strength", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Intelligence", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, Category = "Dexterity", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Dexterity)

	UPROPERTY(BlueprintReadOnly, Category = "Endurance", ReplicatedUsing = OnRep_Endurance)
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Endurance)

	
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
	
	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, MaxMovementSpeed)

	// Secondary
	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "Evasion", ReplicatedUsing = OnRep_Evasion)
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UProjectN_AttributeSet, Evasion)

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

protected:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Primary Attributes
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);
	UFUNCTION()
	virtual void OnRep_Endurance(const FGameplayAttributeData& OldEndurance);

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
	virtual void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed);
	
	// Secondary
	UFUNCTION()
    virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION()
	virtual void OnRep_Evasion(const FGameplayAttributeData& OldEvasion);
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
