// N Chedurov All Rights Reserved


#include "ProjectN_GameInstance.h"

#include "AbilitySystemGlobals.h"

void UProjectN_GameInstance::Init()
{
	Super::Init();

	// Required to use target data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
