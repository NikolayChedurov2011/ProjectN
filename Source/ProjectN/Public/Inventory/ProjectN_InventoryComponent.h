// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryItemsRecord.h"
#include "ProjectN_InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTN_API UProjectN_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProjectN_InventoryComponent();
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void InitializeComponent() override;

	UPROPERTY(Replicated, EditDefaultsOnly)
	FInventoryList InventoryList;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UItemStaticClass>> DefaultItems;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
