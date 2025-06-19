// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/Projectile/ProjectN_WeaponProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actors/ProjectN_ProjectileBase.h"
#include "Interfaces/AvatarInfoInterface.h"
#include "Interfaces/InventoryInterface.h"

void UProjectN_WeaponProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UProjectN_WeaponProjectileAbility::SpawnProjectile() const
{
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
	{
		//ServerSpawnProjectile(TargetLocation);
		return;
	}
	
	if (GetOwningActorFromActorInfo()->Implements<UAvatarInfoInterface>())
	{
		FTransform SpawnTransform;
		const FVector SocketLocation = IAvatarInfoInterface::Execute_GetWeaponSocketLocation(GetOwningActorFromActorInfo(), RequiredSlot);

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());

		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileClassToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		if (GetOwningActorFromActorInfo()->Implements<UInventoryInterface>() && IsValid(DamageEffect))
		{			
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddSourceObject(SpawnedProjectile);
			ContextHandle.SetAbility(this);
			
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), ContextHandle);

			AssignDamageTypes(SpecHandle, IInventoryInterface::Execute_GetWeaponDamageTypes(GetOwningActorFromActorInfo(), RequiredSlot));
			
			SpawnedProjectile->SetDamageEffectHandle(SpecHandle);
		}
		
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}