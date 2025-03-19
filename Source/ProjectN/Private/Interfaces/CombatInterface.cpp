// N Chedurov All Rights Reserved


#include "Interfaces/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetCharacterLevel()
{
	return 0;
}

FVector ICombatInterface::GetWeaponSocketLocation(const FGameplayTag& InputTag)
{
	return FVector::ZeroVector;
}