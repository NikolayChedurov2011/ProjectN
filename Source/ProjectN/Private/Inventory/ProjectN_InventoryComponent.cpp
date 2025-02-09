// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_InventoryComponent.h"

#include "Net/UnrealNetwork.h"

UProjectN_InventoryComponent::UProjectN_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UProjectN_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_InventoryComponent, InventoryList);
}

bool UProjectN_InventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bIsWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FInventoryItem& Item : InventoryList.GetItemsRef())
	{
		UItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			bIsWroteSomething |= ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}
	return bIsWroteSomething;
}

void UProjectN_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (const auto ItemClass : DefaultItems)
        {
        	InventoryList.AddItem(ItemClass);
        }
	}
}

void UProjectN_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

