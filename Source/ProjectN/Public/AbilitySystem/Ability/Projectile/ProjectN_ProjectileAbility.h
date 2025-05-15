// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ProjectN_DamageAbilityBase.h"
#include "ProjectN_ProjectileAbility.generated.h"

class AProjectN_ProjectileBase;
class UGameplayEffect;

UCLASS()
class PROJECTN_API UProjectN_ProjectileAbility : public UProjectN_DamageAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile() const;

	/*
	UFUNCTION(Server, Reliable)
	void ServerSpawnProjectile(const FVector& TargetLocation);

	void SpawnProjectile_Internal(const FVector& TargetLocation) const;
	*/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TSubclassOf<AProjectN_ProjectileBase> ProjectileClassToSpawn = nullptr;
};
