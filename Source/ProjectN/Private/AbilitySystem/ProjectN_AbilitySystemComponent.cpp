// N Chedurov All Rights Reserved


#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Ability/ProjectN_GameplayAbilityBase.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

UProjectN_AbilitySystemComponent::UProjectN_AbilitySystemComponent()
{
	AbilityCommittedCallbacks.AddLambda([this](const UGameplayAbility* Ability)
	{
		FGameplayTag DynamicTag = Ability->GetCurrentAbilitySpec()->GetDynamicSpecSourceTags().First();
		if (DynamicTag.IsValid())
		{
			if (CooldownTags.Find(DynamicTag))
			{
				ServerBroadcastCooldown(*CooldownTags.Find(DynamicTag));
			}
		}
	});
}

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
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag.IsValid()? InputTag : ProjectN_Ability->GetStartupTag());
			return GiveAbility(AbilitySpec);
		}
		return GiveAbility(AbilitySpec);
		
	}
	const FGameplayAbilitySpecHandle EmptyGameplayAbilitySpecHandle;
	return EmptyGameplayAbilitySpecHandle;
}

void UProjectN_AbilitySystemComponent::ServerAddAbility_Implementation(TSubclassOf<UGameplayAbility> DefaultAbility, const FGameplayTag& InputTag, const FGameplayTag& CooldownTag)
{
	AddAbility(DefaultAbility, InputTag);
	ClientAddCooldownTag(InputTag, CooldownTag);
}

void UProjectN_AbilitySystemComponent::ServerRemoveAbility_Implementation(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			ClearAbility(AbilitySpec.Handle);
			ClientRemoveCooldownTag(InputTag);			
		}
	}
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

	FScopedAbilityListLock ActiveScopeLoc(*this);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, Level, InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle;
	}
	
	return EmptyGameplayEffectHandle;
}

void UProjectN_AbilitySystemComponent::OnActionPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{			
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
			else
			{
				if (TryActivateAbility(AbilitySpec.Handle))
				{
					InputTagTriggered.ExecuteIfBound(InputTag);
				}
			}
		}
	}
}

void UProjectN_AbilitySystemComponent::OnActionHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				//if (TryActivateAbility(AbilitySpec.Handle))
				{
					/*for (TTuple<FGameplayTag, FGameplayTag>& Tag : CooldownTags)
					{
						if (Tag.Key == InputTag)
						{
							ServerBroadcastCooldown(*CooldownTags.Find(InputTag));
						}
					}*/
				}
			}
		}
	}
}

void UProjectN_AbilitySystemComponent::ServerBroadcastCooldown_Implementation(const FGameplayTag& CooldownTag)
{
	const float CooldownRemaining = FindCooldownRemaining(CooldownTag);
	ClientBroadcastCooldown(CooldownTag, CooldownRemaining);
}

void UProjectN_AbilitySystemComponent::OnActionReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

bool UProjectN_AbilitySystemComponent::TryActivateActionBarAbility(TSubclassOf<UGameplayAbility> UseItemAbility, const FGameplayTag& CooldownTag, const FName& AbilityIDToActivate)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(UseItemAbility, 1.f);
	if (UProjectN_GameplayAbilityBase* Ability = Cast<UProjectN_GameplayAbilityBase>(AbilitySpec.Ability))
	{
		Ability->SetItemId(AbilityIDToActivate);
	}
	
	if (GiveAbilityAndActivateOnce(AbilitySpec).IsValid())
	{
		const float CooldownRemaining = FindCooldownRemaining(CooldownTag);
		ClientBroadcastCooldown(CooldownTag, CooldownRemaining);

		return true;
	}

	return false;
}

void UProjectN_AbilitySystemComponent::ServerTryActivateActionBarAbility_Implementation(TSubclassOf<UGameplayAbility> UseItemAbility, const FGameplayTag& CooldownTag, const FName& AbilityIDToActivate)
{
	TryActivateActionBarAbility(UseItemAbility, CooldownTag, AbilityIDToActivate);
}

void UProjectN_AbilitySystemComponent::ServerTryAddAbility_Implementation(TSubclassOf<UGameplayAbility> Ability, const FGameplayTag& ActionInputTag, const FGameplayTag& CooldownTag)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(ActionInputTag);
		
	GiveAbility(AbilitySpec);

	ClientAddCooldownTag(ActionInputTag, CooldownTag);
}

void UProjectN_AbilitySystemComponent::ServerTryClearAbility_Implementation(const FGameplayTag& ActionInputTag)
{
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(ActionInputTag))
		{
			AbilitiesToRemove.Add(AbilitySpec.Handle);
		}
	}
	for (FGameplayAbilitySpecHandle& AbilityToRemove : AbilitiesToRemove)
	{
		ClearAbility(AbilityToRemove);
		ClientRemoveCooldownTag(ActionInputTag);	
	}
	AbilitiesToRemove.Empty();
}

void UProjectN_AbilitySystemComponent::ClientBroadcastCooldown_Implementation(const FGameplayTag CooldownTag, const float CooldownRemaining)
{
	OnNewCooldown.ExecuteIfBound(CooldownTag, CooldownRemaining);
}

float UProjectN_AbilitySystemComponent::FindCooldownRemaining(const FGameplayTag CooldownTag) const
{
	const FGameplayEffectQuery ActiveGameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	TArray<float> TimesRemaining = GetActiveEffectsTimeRemaining(ActiveGameplayEffectQuery);
	if (TimesRemaining.Num())
	{
		float TimeRemaining = TimesRemaining[0];

		for (int32 i = 0; i < TimesRemaining.Num(); i++)
		{
			if (TimesRemaining[i] > TimeRemaining)
			{
				TimeRemaining = TimesRemaining[i];
			}
		}
		return TimeRemaining;
	}
	return 0.f;
}

// Use to ADD value to attribute
void UProjectN_AbilitySystemComponent::ServerAddToAttributeByTag_Implementation(const FGameplayTag& AttributeTag, const float Value)
{
	//SendGameplayEventForAttributeWithTag(AttributeTag, Value);

	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = Value;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag,Payload);
	//ProjectNPlayerState->AddToAttributePoints(-Value);
}

void UProjectN_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	bIsAbilityAdded = true;
	AbilitiesGiven.Broadcast(this);
}

void UProjectN_AbilitySystemComponent::ClientAddCooldownTag_Implementation(const FGameplayTag& InputTag, const FGameplayTag& CooldownTag)
{
	CooldownTags.Add(InputTag, CooldownTag);
}

void UProjectN_AbilitySystemComponent::ClientRemoveCooldownTag_Implementation(const FGameplayTag& InputTag)
{
	CooldownTags.FindAndRemoveChecked(InputTag);
}