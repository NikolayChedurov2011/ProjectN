// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_AbilitySystemComponent.generated.h"

class UProjectN_AbilitySystemComponent;
struct FProjectNAttributeSaveInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /* AssetTags */)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGivenSignature, UProjectN_AbilitySystemComponent* /* ProjectN_AbilitySystemComponent */)
DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec& /* GameplayAbilitySpec */)

UCLASS()
class PROJECTN_API UProjectN_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AbilityActorInfoSet();

	FEffectAssetTagsSignature EffectAssetTags;
	FAbilitiesGivenSignature AbilitiesGiven;

	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle AddAbility(const TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag = FGameplayTag());
	UFUNCTION(Server, Reliable)
	void ServerAddAbility(TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag = FGameplayTag());
	UFUNCTION(Server, Reliable)
	void ServerRemoveAbility(const FGameplayTag& InputTag = FGameplayTag());
	FActiveGameplayEffectHandle ApplyGamePlayEffectToSelf_Internal(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext, const float Level);
	FGameplayAbilitySpecHandle AddPassiveAbility(const TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag = FGameplayTag());

	void OnActionPressed(const FGameplayTag& InputTag);
	void OnActionHeld(const FGameplayTag& InputTag);
	void OnActionReleased(const FGameplayTag& InputTag);

	UFUNCTION(Server, Reliable)
	void ServerAddToAttributeByTag(const FGameplayTag& AttributeTag, const float Value);
	void SendGameplayEventForAttributeWithTag(const FGameplayTag& AttributeTag, const float Value = 0.f) const;
	
	void ForEachAbility(const FForEachAbilitySignature& Delegate);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	bool bIsAbilityAdded = false;
	
protected:

	// Broadcast tags from applied effect mainly to show UI message
	UFUNCTION(Client, Reliable)
	void OnEffectApply(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle) const;

	void OnRep_ActivateAbilities() override;
};
