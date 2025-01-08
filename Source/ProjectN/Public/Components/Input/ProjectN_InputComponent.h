// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/InputConfig/DataAsset_InputConfig.h"
#include "ProjectN_InputComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UProjectN_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);
};

template <class UserObject, typename CallbackFunc>
inline void UProjectN_InputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,	const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input config data asset is null."));

	if(const UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}
