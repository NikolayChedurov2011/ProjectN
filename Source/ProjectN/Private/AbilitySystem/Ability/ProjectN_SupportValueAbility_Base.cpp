// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_SupportValueAbility_Base.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_GameplayTags.h"
#include "ProjectN/ProjectNTypes.h"

FGameplayEffectSpecHandle UProjectN_SupportValueAbility_Base::ApplyValue() const
{
	if (!GetOwningActorFromActorInfo()->HasAuthority())
	{
		return FGameplayEffectSpecHandle();
	}

	if (IsValid(Effect))
	{
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		ContextHandle.SetAbility(this);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(Effect, GetAbilityLevel(), ContextHandle);

		AssignValue(SpecHandle);

		return SpecHandle;
	}
	return nullptr;
}

void UProjectN_SupportValueAbility_Base::AssignValue(const FGameplayEffectSpecHandle& EffectSpecHandle) const
{
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*GetEntryManifest(GetItemId())->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
	const FGameplayTag Tag = AbilityFragment->GetAbilityValue().Get<FLabeledFragment>().GetTag();
	const float Value = AbilityFragment->GetAbilityValue().Get<FLabeledFragment>().GetValue();
		
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Tag, Value);
}