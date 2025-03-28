// N Chedurov All Rights Reserved


#include "Interfaces/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetCharacterLevel() const
{
	return 0;
}

FVector ICombatInterface::GetWeaponSocketLocation(const FGameplayTag& InputTag) const
{
	return FVector::ZeroVector;
}