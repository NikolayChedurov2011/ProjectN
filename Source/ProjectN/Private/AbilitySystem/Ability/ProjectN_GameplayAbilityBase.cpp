// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_GameplayAbilityBase.h"

#include "ProjectN_GameplayTags.h"
#include "ProjectN/ProjectNTypes.h"

void UProjectN_GameplayAbilityBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	
	if (!CooldownGE)
	{
		return;
	}

	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
	
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityFragment->GetCooldownTag());
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(TagContainer);
	SpecHandle.Data.Get()->SetSetByCallerMagnitude(AbilityFragment->GetCooldownTag(), AbilityFragment->GetCooldownValue());
	
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

/*******************
*   Getters
********************/
FEntriesDefinition* UProjectN_GameplayAbilityBase::GetEntryManifest(const FName& EntryID) const
{
	if (!Entries) return nullptr;
	
	const FString Context = FString(TEXT("UProjectN_GameplayAbilityBase::FindEntryFromDataTable"));
	
	if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(EntryID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}