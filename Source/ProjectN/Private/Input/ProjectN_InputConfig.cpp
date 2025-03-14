// N Chedurov All Rights Reserved


#include "Input/ProjectN_InputConfig.h"

const UInputAction* UProjectN_InputConfig::FindInputActionByTag(const FGameplayTag& InputTag) const
{
	const FActionInputConfig* FindInputActionConfig = ActionsInputConfig.FindByPredicate([InputTag](const FActionInputConfig& InputActionConfig)
	{
		return InputActionConfig.GameplayTag == InputTag;
	});

	return FindInputActionConfig->InputAction;
}
