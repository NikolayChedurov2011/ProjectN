// N Chedurov All Rights Reserved


#include "Inventory/ItemInstance.h"
#include "Net/UnrealNetwork.h"


void UItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UItemInstance, bIsEquipped);
}

void UItemInstance::Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

void UItemInstance::OnEquipped()
{
}

void UItemInstance::OnUnequipped()
{
}

void UItemInstance::OnRep_IsEquipped()
{
	
}
