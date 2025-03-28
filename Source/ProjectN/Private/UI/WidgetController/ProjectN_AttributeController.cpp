// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AttributeController.h"

#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Data/SaveDataInfo.h"
#include "GameFramework/PlayerState.h"

void UProjectN_AttributeController::BroadcastInitialValues()
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("Fill the attribute info data asset in attribute menu widget controller"))
	
	for(const auto& Pair : Attributes->TagsToAttribute)
	{		
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}

	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
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

TArray<FProjectNAttributeSaveInfo> UProjectN_AttributeController::GetAttributesForSave() const
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	TArray<FProjectNAttributeSaveInfo> AttributeSaveInfo;
	
	for(const TTuple<FGameplayTag, FGameplayAttribute>& Pair : Attributes->TagsToAttribute)
	{
		bool bIsFound = false;
		const float AttributeValue = AbilitySystemComponent->GetGameplayAttributeValue(Pair.Value, bIsFound);
		if (bIsFound)
		{
			FProjectNAttributeSaveInfo WriteSaveInfo;
			WriteSaveInfo.AttributeTag = Pair.Key;
			WriteSaveInfo.AttributeValue = AttributeValue;

			AttributeSaveInfo.Add(WriteSaveInfo);
		}
	}

	return AttributeSaveInfo;
}

void UProjectN_AttributeController::ChangeAttribute(const FGameplayTag& AttributeTag, const int32 Value)
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

void UProjectN_AttributeController::NullifyAttributes() const
{
	if (AbilitySystemComponent && PlayerState->HasAuthority())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
		
		Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->ApplyGamePlayEffectToSelf_Internal(NullifyAttributesEffect, EffectContext, 1.f);
	}
	else
	{		
		ServerNullifyAttributes();
	}
}

void UProjectN_AttributeController::ServerNullifyAttributes_Implementation() const
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	
	Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->ApplyGamePlayEffectToSelf_Internal(NullifyAttributesEffect, EffectContext, 1.f);
}
