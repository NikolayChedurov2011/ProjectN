// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_WeaponAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "Inventory/ProjectN_WeaponActor.h"

/*void UProjectN_WeaponAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!GetOwningActorFromActorInfo()->HasAuthority())
	{
		return;
	}


}*/

void UProjectN_WeaponAbilityBase::StartWeaponHit(const EItemSlot InSlot) const
{
	if (!GetOwningActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	if (GetOwningActorFromActorInfo()->Implements<UInventoryInterface>() && IsValid(DamageEffect))
	{
		AProjectN_WeaponActor* Weapon = IInventoryInterface::Execute_GetEquippedWeaponForSlot(GetOwningActorFromActorInfo(), InSlot);

		if (!IsValid(Weapon))
		{
			return;
		}
		
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		ContextHandle.AddSourceObject(Weapon);
		ContextHandle.SetAbility(this);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), ContextHandle);
		
		AssignDamageTypes(SpecHandle, IInventoryInterface::Execute_GetWeaponDamageTypes(GetOwningActorFromActorInfo(), InSlot));

		Weapon->SetDamageEffectHandle(SpecHandle);

		if (UCapsuleComponent* WeaponCapsule = Weapon->FindComponentByClass<UCapsuleComponent>())
		{
			WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}

void UProjectN_WeaponAbilityBase::StopWeaponHit(const EItemSlot InSlot) const
{
	if (!GetOwningActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	const AProjectN_WeaponActor* Weapon = IInventoryInterface::Execute_GetEquippedWeaponForSlot(GetOwningActorFromActorInfo(), InSlot);
	
	if (UCapsuleComponent* WeaponCapsule = Weapon->FindComponentByClass<UCapsuleComponent>())
	{
		WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}