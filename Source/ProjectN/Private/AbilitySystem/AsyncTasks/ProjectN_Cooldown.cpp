// N Chedurov All Rights Reserved


#include "AbilitySystem/AsyncTasks/ProjectN_Cooldown.h"

#include "AbilitySystemComponent.h"

UProjectN_Cooldown* UProjectN_Cooldown::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,	const FGameplayTag& InCooldownTag)
{
	UProjectN_Cooldown* WaitCooldown = NewObject<UProjectN_Cooldown>();
	WaitCooldown->ASC = AbilitySystemComponent;
	WaitCooldown->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldown->EndTask();
		return nullptr;
	}

	// To know when a cooldown has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldown, &UProjectN_Cooldown::OnActiveEffectAdded);

	// To know when a cooldown has ended
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldown, &UProjectN_Cooldown::CooldownTagChanged);

	return WaitCooldown;
}

void UProjectN_Cooldown::EndTask()
{
	if (!IsValid(ASC))
	{
		return;
	}
	
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UProjectN_Cooldown::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
	else
	{
		
	}
}

void UProjectN_Cooldown::OnActiveEffectAdded(UAbilitySystemComponent* TargetAbilitySystemComponent,	const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	GameplayEffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	GameplayEffectSpec.GetAllGrantedTags(AssetTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery ActiveGameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(ActiveGameplayEffectQuery);
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
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
