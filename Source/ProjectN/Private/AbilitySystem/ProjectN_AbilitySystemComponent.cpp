// N Chedurov All Rights Reserved


#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"

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

FGameplayAbilitySpecHandle UProjectN_AbilitySystemComponent::GiveAbility_Internal(const TSubclassOf<UGameplayAbility> DefaultAbility)
{
	if (IsValid(DefaultAbility))
	{
		return GiveAbility(DefaultAbility);
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

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, Level,InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle;
	}
	
	return EmptyGameplayEffectHandle;
}