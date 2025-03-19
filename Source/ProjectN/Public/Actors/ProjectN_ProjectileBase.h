// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "ProjectN_ProjectileBase.generated.h"

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

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent; 
};
