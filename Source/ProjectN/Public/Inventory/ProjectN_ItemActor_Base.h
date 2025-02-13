// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;  

	void Init(UProjectN_ItemInstance* InItemInstance);
	
	virtual void OnEquipped();
	virtual void OnUnequipped();
	virtual void OnDropped();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	// Pointer to instance of this actor, provides access to static data class 
	UPROPERTY(Replicated)
	TObjectPtr<UProjectN_ItemInstance> ItemInstance = nullptr;
	UPROPERTY(Replicated)
	TEnumAsByte<EItemState> ItemState = EItemState::None;
	
	UPROPERTY(Replicated)
	TObjectPtr<USphereComponent> SphereComponent = nullptr;

	FGameplayTag OverlapEventTag;
};
