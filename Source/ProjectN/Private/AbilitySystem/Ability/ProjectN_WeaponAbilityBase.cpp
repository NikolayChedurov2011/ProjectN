// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/ProjectN_WeaponAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "Inventory/ProjectN_ItemActor_Base.h"

void UProjectN_WeaponAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UProjectN_WeaponAbilityBase::StartWeaponHit() const
{
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
	{
		return;
	}

	if (GetOwningActorFromActorInfo()->Implements<UInventoryInterface>() && IsValid(DamageEffect))
	{
		AProjectN_ItemActor_Base* Weapon = IInventoryInterface::Execute_GetEquippedWeaponForSlot(GetOwningActorFromActorInfo(), RequiredSlot);
		
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
		FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
		ContextHandle.AddSourceObject(Weapon);
		ContextHandle.SetAbility(this);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), ContextHandle);
		
		AssignDamageTypes(SpecHandle, IInventoryInterface::Execute_GetWeaponDamageTypes(GetOwningActorFromActorInfo(), RequiredSlot));
		
		if (UCapsuleComponent* WeaponCapsule = Weapon->FindComponentByClass<UCapsuleComponent>())
		{
			WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		//SpawnedProjectile->SetDamageEffectHandle(SpecHandle);
	}
	
	//SpawnedProjectile->FinishSpawning(SpawnTransform);
	
}

void UProjectN_WeaponAbilityBase::StopWeaponHit() const
{
	AProjectN_ItemActor_Base* Weapon = IInventoryInterface::Execute_GetEquippedWeaponForSlot(GetOwningActorFromActorInfo(), RequiredSlot);
	
	if (UCapsuleComponent* WeaponCapsule = Weapon->FindComponentByClass<UCapsuleComponent>())
	{
		WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
