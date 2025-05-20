// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ProjectN_ProjectileAbilityBase.h"
#include "ProjectN_VersatilityProjectileAbility.generated.h"

class AProjectN_ProjectileBase;
class UGameplayEffect;

UCLASS()
class PROJECTN_API UProjectN_VersatilityProjectileAbility : public UProjectN_ProjectileAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void SpawnProjectile() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	FName SpawnSocket = NAME_None;
};
