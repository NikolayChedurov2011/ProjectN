// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_ItemActor_Base.generated.h"

class USphereComponent;
class UProjectN_ItemInstance;

UCLASS()
class PROJECTN_API AProjectN_ItemActor_Base : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectN_ItemActor_Base();
	
	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetWeaponSocketLocationForProjectile();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	EItemState ItemState = EItemState::None;
	
	UFUNCTION()
	void OnRep_ItemState();
};
