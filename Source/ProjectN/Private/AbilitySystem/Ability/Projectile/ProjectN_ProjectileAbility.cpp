// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/Projectile/ProjectN_ProjectileAbility.h"

#include "Actors/ProjectN_ProjectileBase.h"
#include "Interfaces/AvatarInfoInterface.h"

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
	
	if (GetOwningActorFromActorInfo()->Implements<UAvatarInfoInterface>())
	{
		FTransform SpawnTransform;		
		const FVector SocketLocation = IAvatarInfoInterface::Execute_GetWeaponSocketLocation(GetOwningActorFromActorInfo(), GetCurrentAbilitySpec()->DynamicAbilityTags.First());

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());

		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}

void UProjectN_ProjectileAbility::ServerSpawnProjectile_Implementation(const FVector& TargetLocation)
{
	SpawnProjectile_Internal(TargetLocation);
}

void UProjectN_ProjectileAbility::SpawnProjectile_Internal(const FVector& TargetLocation) const
{
	if (GetOwningActorFromActorInfo()->Implements<UAvatarInfoInterface>())
	{
		FTransform SpawnTransform;
		const FVector SocketLocation = IAvatarInfoInterface::Execute_GetWeaponSocketLocation(GetOwningActorFromActorInfo(), GetCurrentAbilitySpec()->DynamicAbilityTags.First());
		const FRotator Rotation = (TargetLocation - SocketLocation).Rotation();

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
	
		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
