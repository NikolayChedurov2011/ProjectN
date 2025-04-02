// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AttributeController.h"

#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameFramework/PlayerState.h"

void UProjectN_AttributeController::BroadcastInitialValues()
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("Fill the attribute info data asset in attribute menu widget controller"))
	
	for(const auto& Pair : Attributes->TagsToAttribute)
	{		
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}

	const AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	OnAttributePointsChanged.Broadcast(ProjectNPlayerState->GetAttributePoints());
}

void UProjectN_AttributeController::BindCallbacksToResponce()
{
	checkf(AttributeInfo, TEXT("Fill the attribute info data asset in attribute menu widget controller"))
	
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	for(auto& Pair : Attributes->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda([this, Pair] (const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value);
		});
	}

	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	ProjectNPlayerState->OnAttributePointsChanged.AddLambda([this] (const int32 Value)
	{
		OnAttributePointsChanged.Broadcast(Value);
	});
}

void UProjectN_AttributeController::BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const
{
	FProjectNAttributeInfo AttributeInfoElem = AttributeInfo->GetAttributeInfoByTag(InTag);
	AttributeInfoElem .AttributeValue = InAttribute.GetNumericValue(AttributeSet);

	AttributeInfoDelegate.Broadcast(AttributeInfoElem);
}

void UProjectN_AttributeController::ChangeAttribute(const FGameplayTag& AttributeTag, const float Value)
{
	// @TODO: Check if attribute + value is not < attribute. Need to load saved attributes. Maybe need bool for return
	if (AbilitySystemComponent && PlayerState->HasAuthority())
	{
		Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->SendGameplayEventWithTag(AttributeTag, Value);
	}
	else
	{
		Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->ServerSendGameplayEventWithTag(AttributeTag, Value);
	}
}