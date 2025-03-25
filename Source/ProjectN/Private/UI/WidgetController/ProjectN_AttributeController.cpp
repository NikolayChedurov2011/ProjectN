// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AttributeController.h"

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
}

void UProjectN_AttributeController::BindCallbacksToResponce()
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("Fill the attribute info data asset in attribute menu widget controller"))
	
	for(auto& Pair : Attributes->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda([this, Pair] (const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value);
		});
	}
}

void UProjectN_AttributeController::BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const
{
	FProjectNAttributeInfo AttributeInfoElem = AttributeInfo->GetAttributeInfoByTag(InTag);
	AttributeInfoElem .AttributeValue = InAttribute.GetNumericValue(AttributeSet);

	AttributeInfoDelegate.Broadcast(AttributeInfoElem);
}

void UProjectN_AttributeController::SaveAttributes()
{
	
}

void UProjectN_AttributeController::ChangeAttribute(const FGameplayTag& AttributeTag, const float Value) const
{
	// @TODO: Check if attribute + value is not < attribute. Need to load saved attributes. Maybe need bool for return
	if (AbilitySystemComponent && PlayerState->HasAuthority())
	{
		CreateEffect(AttributeTag, Value);
	}
	else
	{
		ServerCreateEffect(AttributeTag, Value);
	}
}

void UProjectN_AttributeController::ServerCreateEffect_Implementation(const FGameplayTag& AttributeTag, const float Value) const
{
	CreateEffect(AttributeTag, Value);
}

void UProjectN_AttributeController::CreateEffect(const FGameplayTag& AttributeTag, const float Value) const
{
	UProjectN_AttributeSet* Attributes = CastChecked<UProjectN_AttributeSet>(AttributeSet);

	UGameplayEffect* EffectTemplate = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GE_ChangeAttribute")));
		
	EffectTemplate->DurationPolicy = EGameplayEffectDurationType::Instant;
		
	const int32 ModIdx = EffectTemplate->Modifiers.Num();
	EffectTemplate->Modifiers.SetNum(ModIdx + 1);

	FGameplayModifierInfo& Modifier = EffectTemplate->Modifiers[ModIdx];
	Modifier.Attribute = *Attributes->TagsToAttribute.Find(AttributeTag);
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.ModifierMagnitude = FScalableFloat(Value);
	//Modifier.SourceTags.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("AttributePreset")));

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectTemplate->GetClass(), 1.f, AbilitySystemComponent->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}