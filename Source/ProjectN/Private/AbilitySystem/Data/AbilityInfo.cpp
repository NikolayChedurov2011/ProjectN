// N Chedurov All Rights Reserved


#include "AbilitySystem/Data/AbilityInfo.h"

FProjectNActionSlotInfo2& UAbilityInfo::GetAbilityInfoByIndex(const int32 InActionSlotIndex)
{
	return *AbilityInformation.FindByPredicate([InActionSlotIndex](const FProjectNActionSlotInfo2& SpellInfo)
	{
		return SpellInfo.ActionSlotIndex == InActionSlotIndex;
	});
}