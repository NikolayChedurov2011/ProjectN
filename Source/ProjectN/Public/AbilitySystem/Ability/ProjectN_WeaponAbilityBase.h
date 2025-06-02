// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_DamageAbilityBase.h"
#include "ProjectN_WeaponAbilityBase.generated.h"

class AProjectN_WeaponActor;

UCLASS()
class PROJECTN_API UProjectN_WeaponAbilityBase : public UProjectN_DamageAbilityBase
{
	GENERATED_BODY()
	
protected:

	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartWeaponHit(const EEquipSlot InSlot) const;

	UFUNCTION(BlueprintCallable)
	virtual void StopWeaponHit(const EEquipSlot InSlot) const;

private:
	
	//TObjectPtr<AProjectN_WeaponActor> Weapon = nullptr;
};
