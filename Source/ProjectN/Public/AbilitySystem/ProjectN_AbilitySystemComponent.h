// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_AbilitySystemComponent.generated.h"

class UProjectN_AbilitySystemComponent;
struct FProjectNAttributeSaveInfo;
struct FTagValueData;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /* AssetTags */)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGivenSignature, UProjectN_AbilitySystemComponent* /* ProjectN_AbilitySystemComponent */)
DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec& /* GameplayAbilitySpec */)
DECLARE_DELEGATE_OneParam(FInputTagTriggeredSignature, const FGameplayTag /* InputTag */)
DECLARE_DELEGATE_TwoParams(FOnNewCooldownSignature, const FGameplayTag /* CooldownTag */, const float /* RemainingValue */)

UCLASS()
class PROJECTN_API UProjectN_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UProjectN_AbilitySystemComponent();
	void AbilityActorInfoSet();

	FEffectAssetTagsSignature EffectAssetTags;
	FAbilitiesGivenSignature AbilitiesGiven;
	FInputTagTriggeredSignature InputTagTriggered;
	FOnNewCooldownSignature OnNewCooldown;

	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle AddAbility(const TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag = FGameplayTag());
	UFUNCTION(Server, Reliable)
	void ServerAddAbility(TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag = FGameplayTag(), const FGameplayTag& CooldownTag = FGameplayTag());
	UFUNCTION(Client, Reliable)
	void ClientAddCooldownTag(const FGameplayTag& InputTag = FGameplayTag(), const FGameplayTag& CooldownTag = FGameplayTag());
	UFUNCTION(Client, Reliable)
	void ClientRemoveCooldownTag(const FGameplayTag& InputTag = FGameplayTag());
	UFUNCTION(Server, Reliable)
	void ServerRemoveAbility(const FGameplayTag& InputTag = FGameplayTag());
	UFUNCTION(Server, Reliable)
	void ServerBroadcastCooldown(const FGameplayTag& CooldownTag = FGameplayTag());
	FActiveGameplayEffectHandle ApplyGamePlayEffectToSelf_Internal(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext, const float Level);
	FGameplayAbilitySpecHandle AddPassiveAbility(const TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag = FGameplayTag());

	void OnActionPressed(const FGameplayTag& InputTag);
	void OnActionHeld(const FGameplayTag& InputTag);
	void OnActionReleased(const FGameplayTag& InputTag);

	UFUNCTION(Server, Reliable)
	void ServerTryActivateActionBarAbility(TSubclassOf<UGameplayAbility> UseItemAbility, const FGameplayTag& CooldownTag, const FName& AbilityIDToActivate);
	UFUNCTION(Server, Reliable)
	void ServerTryAddAbility(TSubclassOf<UGameplayAbility> Ability, const FGameplayTag& ActionInputTag, const FGameplayTag& CooldownTag);
	UFUNCTION(Server, Reliable)
	void ServerTryClearAbility(const FGameplayTag& ActionInputTag);
	bool TryActivateActionBarAbility(TSubclassOf<UGameplayAbility> UseItemAbility, const FGameplayTag& CooldownTag, const FName& AbilityIDToActivate);
	bool OnAbilityActivated(const FGameplayTag& CooldownTag);

	UFUNCTION(Server, Reliable)
	void ServerAddToAttributeByTag(const FGameplayTag& AttributeTag, const float Value);

	bool bIsAbilityAdded = false;

	UFUNCTION(Client, Reliable)
	void ClientBroadcastCooldown(const FGameplayTag CooldownTag, const float CooldownRemaining);
	float FindCooldownRemaining(const FGameplayTag CooldownTag) const;
	
protected:

	// Broadcast tags from applied effect mainly to show UI message
	UFUNCTION(Client, Reliable)
	void OnEffectApply(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle) const;

	void OnRep_ActivateAbilities() override;

private:

	TMap<FGameplayTag, FGameplayTag> CooldownTags{};
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove{};
};
