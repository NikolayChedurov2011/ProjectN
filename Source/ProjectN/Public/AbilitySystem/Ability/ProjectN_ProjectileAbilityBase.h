// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_DamageAbilityBase.h"
#include "ProjectN_ProjectileAbilityBase.generated.h"

class AProjectN_ProjectileBase;
class UGameplayEffect;

UCLASS()
class PROJECTN_API UProjectN_ProjectileAbilityBase : public UProjectN_DamageAbilityBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TSubclassOf<AProjectN_ProjectileBase> ProjectileClassToSpawn = nullptr;
};
