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
		//ServerSpawnProjectile(TargetLocation);
		return;
	}
	
	//SpawnProjectile_Internal(TargetLocation);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwningActorFromActorInfo());
	if (CombatInterface)
	{
		FTransform SpawnTransform;
		const FVector SocketLocation = CombatInterface->GetWeaponSocketLocation(GetCurrentAbilitySpec()->DynamicAbilityTags.First());
		//const FRotator Rotation = (TargetLocation - SocketLocation).Rotation();

		SpawnTransform.SetLocation(SocketLocation);
		//SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());
	
		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}

void UProjectN_ProjectileAbility::ServerSpawnProjectile_Implementation(const FVector& TargetLocation)
{
	SpawnProjectile_Internal(TargetLocation);
}

void UProjectN_ProjectileAbility::SpawnProjectile_Internal(const FVector& TargetLocation)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwningActorFromActorInfo());
	if (CombatInterface)
	{
		FTransform SpawnTransform;
		const FVector SocketLocation = CombatInterface->GetWeaponSocketLocation(GetCurrentAbilitySpec()->DynamicAbilityTags.First());
		const FRotator Rotation = (TargetLocation - SocketLocation).Rotation();

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
	
		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
