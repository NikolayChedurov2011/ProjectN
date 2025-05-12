// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "ProjectN_ProjectileBase.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PROJECTN_API AProjectN_ProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectN_ProjectileBase();

	UFUNCTION(BlueprintPure)
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FGameplayEffectSpecHandle& GetDamageEffectHandle() { return DamageEffectHandle; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDamageEffectHandle(const FGameplayEffectSpecHandle& Handle) { DamageEffectHandle = Handle; }

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:

	void SpawnImpactSoundAndEffect() const;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ProjectileSoundComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ProjectileSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	FGameplayEffectSpecHandle DamageEffectHandle;

	bool bHit = false;
	float LifeSpan = 5.f;
};
