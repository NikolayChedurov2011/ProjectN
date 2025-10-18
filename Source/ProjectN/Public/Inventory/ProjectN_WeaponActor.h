// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_ItemActor_Base.h"
#include "ProjectN_WeaponActor.generated.h"

class UCapsuleComponent;

UCLASS()
class PROJECTN_API AProjectN_WeaponActor : public AProjectN_ItemActor_Base
{
	GENERATED_BODY()

public:
	AProjectN_WeaponActor();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDamageEffectHandle(const FGameplayEffectSpecHandle& Handle) { DamageEffectHandle = Handle; }

protected:
	//virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:

	void SpawnImpactSoundAndEffect() const;
	
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UCapsuleComponent> CapsuleComponent;

	FGameplayEffectSpecHandle DamageEffectHandle;
};
