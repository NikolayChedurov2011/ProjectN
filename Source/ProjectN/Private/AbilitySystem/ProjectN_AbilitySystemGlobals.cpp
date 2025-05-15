// N Chedurov All Rights Reserved


#include "AbilitySystem/ProjectN_AbilitySystemGlobals.h"

#include "ProjectN/ProjectNTypes.h"

FGameplayEffectContext* UProjectN_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FProjectNGameplayEffectContext();
}
