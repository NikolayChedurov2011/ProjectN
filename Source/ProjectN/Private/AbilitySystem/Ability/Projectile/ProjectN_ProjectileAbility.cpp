// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/Projectile/ProjectN_ProjectileAbility.h"

#include "Actors/ProjectN_ProjectileBase.h"
#include "Interfaces/CombatInterface.h"

void UProjectN_ProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UProjectN_ProjectileAbility::SpawnProjectile()
{
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
	{
		ServerSpawnProjectile();
		return;
	}
	
	SpawnProjectile_Internal();
}

void UProjectN_ProjectileAbility::ServerSpawnProjectile_Implementation()
{
	SpawnProjectile_Internal();
}

void UProjectN_ProjectileAbility::SpawnProjectile_Internal()
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwningActorFromActorInfo());
	if (CombatInterface)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatInterface->GetWeaponSocketLocation(GetCurrentAbilitySpec()->DynamicAbilityTags.First()));
	
		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
