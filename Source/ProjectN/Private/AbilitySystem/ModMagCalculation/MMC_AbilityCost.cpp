// N Chedurov All Rights Reserved


#include "AbilitySystem/ModMagCalculation/MMC_AbilityCost.h"

#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/Ability/ProjectN_GameplayAbilityBase.h"
#include "ProjectN/ProjectNTypes.h"


float UMMC_AbilityCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(Cast<UProjectN_GameplayAbilityBase>(Spec.GetContext().GetAbility())->GetItemId());
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
	
	
	return AbilityFragment->GetCostValue();
}

FEntriesDefinition* UMMC_AbilityCost::GetEntryManifest(const FName& EntryID) const
{
	if (!Entries) return nullptr;
	
	const FString Context = FString(TEXT("UMMC_AbilityCost::FindEntryFromDataTable"));
	
	if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(EntryID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}
