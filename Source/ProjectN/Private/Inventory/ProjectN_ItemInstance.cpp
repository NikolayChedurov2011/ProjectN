// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_ItemInstance.h"

#include "GameFramework/Character.h"
#include "Inventory/ProjectN_ItemActor_Base.h"
#include "Net/UnrealNetwork.h"
#include "ProjectN_Statics.h"


void UProjectN_ItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_ItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UProjectN_ItemInstance, bIsEquipped);
	DOREPLIFETIME(UProjectN_ItemInstance, ItemActor);
}

void UProjectN_ItemInstance::Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

const UItemStaticClass* UProjectN_ItemInstance::GetItemStaticClass() const
{
	return UProjectN_Statics::GetItemStaticData(ItemStaticDataClass);
}

void UProjectN_ItemInstance::OnEquip(AActor* Owner)
{
	if (UWorld* World = Owner->GetWorld())
	{
		const FTransform Transform;
		
		ItemActor = World->SpawnActorDeferred<AProjectN_ItemActor_Base>(GetItemStaticClass()->GetItemActorClass(), Transform, Owner);
		ItemActor->Init(this);
		ItemActor->OnEquipped();
		ItemActor->FinishSpawning(Transform);

		const ACharacter* Character = Cast<ACharacter>(Owner);

		if (USkeletalMeshComponent* SkeletalMeshComponent = Character ? Character->GetMesh() : nullptr)
		{
			ItemActor->AttachToComponent(SkeletalMeshComponent,  FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetItemStaticClass()->GetSocketToAttach());
		}
	}
	
	bIsEquipped = true;
}

void UProjectN_ItemInstance::OnUnEquip()
{
	if (IsValid(ItemActor))
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
	
	bIsEquipped = false;
}

void UProjectN_ItemInstance::OnDrop()
{
	if (IsValid(ItemActor))
	{
		ItemActor->OnDropped();
	}

	bIsEquipped = false;
}

void UProjectN_ItemInstance::OnRep_IsEquipped()
{
	
}
