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
