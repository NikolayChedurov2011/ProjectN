// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_DamageAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"

void UProjectN_DamageAbilityBase::AssignDamageTypes(const FGameplayEffectSpecHandle& EffectSpecHandle, TMap<FGameplayTag, float> SourceDamageTypes) const
{
	for (const auto DamageType : SourceDamageTypes)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType.Key, DamageType.Value);
	}
}
