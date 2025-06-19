// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_DamageAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_GameplayTags.h"

void UProjectN_DamageAbilityBase::AssignDamageTypes(const FGameplayEffectSpecHandle& EffectSpecHandle, TMap<FGameplayTag, float> SourceDamageTypes) const
{
	for (const auto DamageType : SourceDamageTypes)
	{
		const float TotalValue = DamageType.Key.MatchesTagExact(ProjectNGameplayTags::DamageType_Physical) ? DamageType.Value * PhysicalDamageMultiplier: DamageType.Value * 1;
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType.Key, TotalValue);
	}
}
