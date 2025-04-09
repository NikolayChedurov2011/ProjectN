// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AttributeController.h"

#include "ProjectN_GameInstance.h"
#include "ProjectN_PlayerCharacter.h"
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
	//OnAttributePointsChanged.Broadcast(static_cast<float>(ProjectNPlayerState->GetAttributePoints()));
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
	ProjectNPlayerState->OnSkillTreePointsChanged.AddLambda([this] (const int32 Value)
	{
		OnAttributePointsChanged.Broadcast(static_cast<float>(Value));
	});
}

void UProjectN_AttributeController::BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const
{
	FProjectNAttributeInfo AttributeInfoElem = AttributeInfo->GetAttributeInfoByTag(InTag);
	AttributeInfoElem .AttributeValue = InAttribute.GetNumericValue(AttributeSet);

	AttributeInfoDelegate.Broadcast(AttributeInfoElem);
}

void UProjectN_AttributeController::AddToAttributeByTag(const FGameplayTag& AttributeTag, const float Value)
{
	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	
	if (ProjectNPlayerState->GetSkillTreePoints() - Value >= 0 && ProjectNPlayerState->GetSkillTreePoints() - Value <= ProjectNPlayerState->GetSkillTreePointsInUse())
	{
		if (PreSavedAttributes.Find(AttributeTag))
		{
			if (*PreSavedAttributes.Find(AttributeTag) + Value >= 0)
			{
				*PreSavedAttributes.Find(AttributeTag) += Value;
				Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->ServerAddToAttributeByTag(AttributeTag, Value);
				ProjectNPlayerState->AddToSkillTreePoints(-Value);
			}
		}
		else if (Value > 0)
		{
			PreSavedAttributes.Add(AttributeTag, Value);
			Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->ServerAddToAttributeByTag(AttributeTag, Value);
			ProjectNPlayerState->AddToSkillTreePoints(-Value);
		}
	}
}

void UProjectN_AttributeController::SaveAttributes()
{	
	AProjectN_PlayerState* ProjectNPlayerState = CastChecked<AProjectN_PlayerState>(PlayerState);
	
	//ProjectNPlayerState->ServerSetAttributePointsInUse(ProjectNPlayerState->GetAttributePoints());
	//Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent.Get())->ClearPreSavedAttributeValues();
}

void UProjectN_AttributeController::RestoreAttributes()
{
	const FString SlotName = Cast<UProjectN_GameInstance>(PlayerState->GetGameInstance())->CurrentSaveSlotName;
	const int32 Index = Cast<UProjectN_GameInstance>(PlayerState->GetGameInstance())->CurrentSaveSlotIndex;
	Cast<AProjectN_PlayerCharacter>(PlayerController->GetPawn())->ApplyAttributesFromSave(SlotName, Index);
}