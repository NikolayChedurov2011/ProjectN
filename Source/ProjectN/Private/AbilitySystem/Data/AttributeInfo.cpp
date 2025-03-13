// N Chedurov All Rights Reserved


#include "AbilitySystem/Data/AttributeInfo.h"

FProjectNAttributeInfo& UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag& Tag)
{
	return *AttributeInformation.FindByPredicate([Tag](const FProjectNAttributeInfo& AttributeInfo)
	{
		return AttributeInfo.AttributeTag == Tag;
	});
}
