// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_GameplayAbilityBase.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectN_DamageAbilityBase.generated.h"

UCLASS()
class PROJECTN_API UProjectN_DamageAbilityBase : public UProjectN_GameplayAbilityBase
{
	GENERATED_BODY()

protected:

	void AssignDamageTypes(const FGameplayEffectSpecHandle& EffectSpecHandle, TMap<FGameplayTag, float> SourceDamageTypes) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TSubclassOf<UGameplayEffect> DamageEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TMap<FGameplayTag, float> DamageTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	bool bUseWeaponDamageTypes = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	bool bHaveCooldown = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	float PhysicalDamageMultiplier = 1.f;
};