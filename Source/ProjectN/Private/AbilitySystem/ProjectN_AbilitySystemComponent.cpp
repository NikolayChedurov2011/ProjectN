// N Chedurov All Rights Reserved


#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_PlayerState.h"
#include "AbilitySystem/Ability/ProjectN_GameplayAbilityBase.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

void UProjectN_AbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UProjectN_AbilitySystemComponent::OnEffectApply);
}

void UProjectN_AbilitySystemComponent::OnEffectApply_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle) const
{
	FGameplayTagContainer AssetTagsContainer;
	GameplayEffectSpec.GetAllAssetTags(AssetTagsContainer);

	EffectAssetTags.Broadcast(AssetTagsContainer);
}

FGameplayAbilitySpecHandle UProjectN_AbilitySystemComponent::AddAbility(const TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag)
{
	if (IsValid(DefaultAbility))
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DefaultAbility, 1.f);
		if (const UProjectN_GameplayAbilityBase* ProjectN_Ability = Cast<UProjectN_GameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(InputTag.IsValid()? InputTag : ProjectN_Ability->GetStartupTag());
			return GiveAbility(AbilitySpec);
		}
		return GiveAbility(AbilitySpec);
		
	}
	const FGameplayAbilitySpecHandle EmptyGameplayAbilitySpecHandle;
	return EmptyGameplayAbilitySpecHandle;
}

FGameplayAbilitySpecHandle UProjectN_AbilitySystemComponent::AddPassiveAbility(const TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag)
{
	if (IsValid(DefaultAbility))
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DefaultAbility, 1.f);
		return GiveAbilityAndActivateOnce(AbilitySpec);
	}
	const FGameplayAbilitySpecHandle EmptyGameplayAbilitySpecHandle;
	return EmptyGameplayAbilitySpecHandle;
}

FActiveGameplayEffectHandle UProjectN_AbilitySystemComponent::ApplyGamePlayEffectToSelf_Internal(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext, const float Level)
{
	const FActiveGameplayEffectHandle EmptyGameplayEffectHandle;
	
	if (!Effect.Get())
	{
		return EmptyGameplayEffectHandle;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, Level, InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle;
	}
	
	return EmptyGameplayEffectHandle;
}

void UProjectN_AbilitySystemComponent::OnActionHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UProjectN_AbilitySystemComponent::OnActionReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

// Use to ADD value to attribute
void UProjectN_AbilitySystemComponent::SendGameplayEventForAttributeWithTag(const FGameplayTag& AttributeTag, const float Value) const
{
	
}

void UProjectN_AbilitySystemComponent::ServerAddToAttributeByTag_Implementation(const FGameplayTag& AttributeTag, const float Value)
{
	//SendGameplayEventForAttributeWithTag(AttributeTag, Value);
	

		FGameplayEventData Payload;
		Payload.EventTag = AttributeTag;
		Payload.EventMagnitude = Value;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag,Payload);
		//ProjectNPlayerState->AddToAttributePoints(-Value);

	
}