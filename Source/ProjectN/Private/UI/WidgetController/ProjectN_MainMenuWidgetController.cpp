// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_MainMenuWidgetController.h"

#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

void UProjectN_MainMenuWidgetController::BroadcastInitialValues()
{
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);
}

void UProjectN_MainMenuWidgetController::BindCallbacksToResponce()
{	
	const UProjectN_AttributeSet* ProjectN_AttributeSet = CastChecked<UProjectN_AttributeSet>(AttributeSet);
	
	Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([this] (const FGameplayTagContainer& EffectAssetTags)
	{
		for (const FGameplayTag& Tag : EffectAssetTags)
		{
			const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
				
			}
		}
	});
}