// N Chedurov All Rights Reserved


#include "AbilitySystem/Data/AbilityInfo.h"

FProjectNAbilityInfo& UAbilityInfo::GetAbilityInfoByTag(const FGameplayTag& AbilityTag)
{
	return *AbilityInformation.FindByPredicate([AbilityTag](const FProjectNAbilityInfo& SpellInfo)
	{
		return SpellInfo.AbilityTag == AbilityTag;
	});
}