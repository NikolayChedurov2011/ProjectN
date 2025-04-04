// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_AttributePoints.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Interfaces/AvatarInfoInterface.h"

float UMMC_AttributePoints::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UAvatarInfoInterface>())
	{
		Level = IAvatarInfoInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}
	
	return Level - 1;
}
