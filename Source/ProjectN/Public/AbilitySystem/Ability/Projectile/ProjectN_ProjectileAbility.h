// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ProjectN_GameplayAbilityBase.h"
#include "ProjectN_ProjectileAbility.generated.h"

class AProjectN_ProjectileBase;

UCLASS()
class PROJECTN_API UProjectN_ProjectileAbility : public UProjectN_GameplayAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();

	UFUNCTION(Server)
	void ServerSpawnProjectile();

	void SpawnProjectile_Internal();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TSubclassOf<AProjectN_ProjectileBase> ProjectileToSpawn = nullptr;
};
