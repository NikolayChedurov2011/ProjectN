// N Chedurov All Rights Reserved


#include "AbilitySystem/Data/LevelUpDataInfo.h"

int32 ULevelUpDataInfo::GetLevelByXP(const int32 XP)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInformationContainer.Num() - 1 < Level)
		{
			return Level;
		}

		if (XP >= LevelUpInformationContainer[Level].XPForLevelUp)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}

int32 ULevelUpDataInfo::GetXPForNextLevelUpByLevel(const int32 Level)
{
	if (Level - 1 == 0)
	{
		return LevelUpInformationContainer[Level].XPForLevelUp;
	}
	const int32 CurrentLevelRequirement = LevelUpInformationContainer[Level].XPForLevelUp;
	const int32 PastLevelRequirement = LevelUpInformationContainer[Level - 1].XPForLevelUp;

	return CurrentLevelRequirement - PastLevelRequirement; 
}
