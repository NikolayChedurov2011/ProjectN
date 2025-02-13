// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_InventoryComponent.h"

#include "Engine/ActorChannel.h"
#include "GameFramework/PlayerState.h"
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
	DOREPLIFETIME(UProjectN_InventoryComponent, CurrentItemInstance);
}

bool UProjectN_InventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bIsWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryItem& Item : InventoryList.GetItemsRef())
	{
		UProjectN_ItemInstance* ItemInstance = Item.ItemInstance;

		if (IsValid(ItemInstance))
		{
			bIsWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}
	return bIsWroteSomething;
}

void UProjectN_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (const auto& ItemClass : DefaultItems)
        {
        	InventoryList.AddItem(ItemClass);
        }
	}
}
void UProjectN_InventoryComponent::EquipTestItem()
{
	if (InventoryList.GetItemsRef().Num() && GetOwner()->HasAuthority())
	{
		EquipItem(InventoryList.GetItemsRef()[0].ItemInstance->GetItemStaticSubClass());
	}
}

void UProjectN_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProjectN_InventoryComponent::AddItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(ItemStaticDataClass);
	}
}

void UProjectN_InventoryComponent::RemoveItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(ItemStaticDataClass);
	}
}

void UProjectN_InventoryComponent::EquipItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority() && IsValid(Cast<APlayerState>(GetOwner())->GetPawn()))
	{
		for (FInventoryItem Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance->GetItemStaticSubClass() == ItemStaticDataClass)
			{
				Item.ItemInstance->OnEquip(Cast<APlayerState>(GetOwner())->GetPawn());
                CurrentItemInstance = Item.ItemInstance;
                break;
			}
		}
	}
}

void UProjectN_InventoryComponent::UnEquipItem(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (FInventoryItem Item : InventoryList.GetItemsRef())
		{
			Item.ItemInstance->OnUnEquip();
			CurrentItemInstance = nullptr;
			break;
		}
	}
}

void UProjectN_InventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItemInstance))
		{
			CurrentItemInstance->OnDrop();
			RemoveItem(CurrentItemInstance->GetItemStaticSubClass());
			CurrentItemInstance = nullptr;
		}
	}
}
