// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AttributeController.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UProjectN_AttributeController::BroadcastInitialValues()
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("Fill the attribute info data asset in attribute menu widget controller"))
	
	for(const auto& Pair : Attributes->TagsToAttribute)
	{		
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UProjectN_AttributeController::BindCallbacksToResponce()
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("Fill the attribute info data asset in attribute menu widget controller"))
	
	for(auto& Pair : Attributes->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair] (const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}
}

void UProjectN_AttributeController::BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const
{
	FProjectNAttributeInfo AttributeInfoElem = AttributeInfo->GetAttributeInfoByTag(InTag);
	AttributeInfoElem .AttributeValue = InAttribute.GetNumericValue(AttributeSet);

	AttributeInfoDelegate.Broadcast(AttributeInfoElem);
}
