// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_ItemActor_Base.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/ProjectN_ItemInstance.h"

AProjectN_ItemActor_Base::AProjectN_ItemActor_Base()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AProjectN_ItemActor_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectN_ItemActor_Base, ItemInstance);
}

bool AProjectN_ItemActor_Base::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bIsWroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	bIsWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	
	return bIsWroteSomething;
}

void AProjectN_ItemActor_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectN_ItemActor_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectN_ItemActor_Base::Init(UProjectN_ItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
}

void AProjectN_ItemActor_Base::OnEqipped()
{
	
}

void AProjectN_ItemActor_Base::OnUneqipped()
{
	
}

void AProjectN_ItemActor_Base::OnDropped()
{
	
}
