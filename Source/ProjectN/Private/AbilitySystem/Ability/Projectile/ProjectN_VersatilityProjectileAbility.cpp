// N Chedurov All Rights Reserved


#include "AbilitySystem/Ability/Projectile/ProjectN_VersatilityProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actors/ProjectN_ProjectileBase.h"
#include "Interfaces/AvatarInfoInterface.h"

void UProjectN_VersatilityProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UProjectN_VersatilityProjectileAbility::SpawnProjectile() const
{
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
	{
		return;
	}
	
	if (GetAvatarActorFromActorInfo()->Implements<UAvatarInfoInterface>())
	{
		FTransform SpawnTransform;
		const FVector SocketLocation = IAvatarInfoInterface::Execute_GetAvatarSocketLocation(GetAvatarActorFromActorInfo(), SpawnSocket);

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(GetAvatarActorFromActorInfo()->GetActorRotation().Quaternion());

		AProjectN_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectN_ProjectileBase>(ProjectileClassToSpawn, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()),  ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		if (IsValid(DamageEffect))
		{			
			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddSourceObject(SpawnedProjectile);
			ContextHandle.SetAbility(this);
			
			const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), ContextHandle);
			
			AssignDamageTypes(SpecHandle, DamageTypes);
			
			SpawnedProjectile->SetDamageEffectHandle(SpecHandle);
		}
		
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}