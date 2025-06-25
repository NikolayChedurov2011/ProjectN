// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_DamageAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_GameplayTags.h"
#include "Interfaces/InventoryInterface.h"
#include "ProjectN/ProjectNTypes.h"

void UProjectN_DamageAbilityBase::AssignDamageTypes(const FGameplayEffectSpecHandle& EffectSpecHandle) const
{
	float Value = 0.f;
	
	if (bUseWeaponDamageTypes)
	{
		for (const auto DamageType : IInventoryInterface::Execute_GetWeaponDamageTypes(GetOwningActorFromActorInfo(), RequiredSlot))
		{
			Value = DamageType.Key.MatchesTagExact(ProjectNGameplayTags::DamageType_Physical) ? DamageType.Value * PhysicalDamageMultiplier: DamageType.Value * 1;
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageType.Key, Value);
		}
	}
	else
	{
		const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*GetEntryManifest(GetItemId())->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
		const FGameplayTag Tag = AbilityFragment->GetAbilityValue().Get<FLabeledFragment>().GetTag();
		Value = AbilityFragment->GetAbilityValue().Get<FLabeledFragment>().GetValue();
		
		const float TotalValue = Tag.MatchesTagExact(ProjectNGameplayTags::DamageType_Physical) ? Value * PhysicalDamageMultiplier: Value * 1;
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Tag, TotalValue);
	}
}