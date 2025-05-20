// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_DamageAbilityBase.h"
#include "ProjectN_WeaponAbilityBase.generated.h"

UCLASS()
class PROJECTN_API UProjectN_WeaponAbilityBase : public UProjectN_DamageAbilityBase
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Blueprintable)
	virtual void StartWeaponHit() const;

	UFUNCTION(Blueprintable)
	virtual void StopWeaponHit() const;
};
