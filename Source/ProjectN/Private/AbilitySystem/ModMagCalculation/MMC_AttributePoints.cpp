// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_AttributePoints.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/CombatInterface.h"

float UMMC_AttributePoints::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 Level = CombatInterface->GetCharacterLevel();
	
	return Level - 1;
}
