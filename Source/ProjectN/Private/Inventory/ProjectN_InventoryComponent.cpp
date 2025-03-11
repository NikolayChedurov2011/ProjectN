// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "ProjectN_CharacterBase.h"
#include "ProjectN_GameplayTags.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

//FGameplayTag UProjectN_InventoryComponent::EquipItemTag;
//FGameplayTag UProjectN_InventoryComponent::UnEquipItemTag;
//FGameplayTag UProjectN_InventoryComponent::DropItemTag;

/*
 **************
 * Initialize
 **************
 */

UProjectN_InventoryComponent::UProjectN_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	//UGameplayTagsManager().Get().OnLastChanceToAddNativeTags().AddUObject(this, &UProjectN_InventoryComponent::AddInventoryTags);
}

void UProjectN_InventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	//UProjectN_InventoryComponent::EquipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Inventory.EquipItem"), TEXT("Equip item"));
	//UProjectN_InventoryComponent::UnEquipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Inventory.UnEquipItem"), TEXT("Un equip item"));
	//UProjectN_InventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Inventory.DropItem"), TEXT("Drop item"));
	
	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

void UProjectN_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_InventoryComponent, InventoryList);
	DOREPLIFETIME(UProjectN_InventoryComponent, CurrentItemInstance);
	DOREPLIFETIME(UProjectN_InventoryComponent, EquippedItemSlots);
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
		for (const TSubclassOf<UItemStaticClass>& ItemClass : DefaultItems)
        {
        	InventoryList.AddItemByStaticClass(ItemClass);
        }
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ProjectNGameplayTags::InventoryTag_Equip).AddUObject(this, &UProjectN_InventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ProjectNGameplayTags::InventoryTag_UnEquip).AddUObject(this, &UProjectN_InventoryComponent::GameplayEventCallback);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ProjectNGameplayTags::InventoryTag_Drop).AddUObject(this, &UProjectN_InventoryComponent::GameplayEventCallback);
	}
}

void UProjectN_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

/*
 **************************************
 * Main functions to handle inventory
 **************************************
 */

void UProjectN_InventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();
	
	if (NetRole == ROLE_Authority)
	{
		HandleGameplayEventInternal(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}

void UProjectN_InventoryComponent::HandleGameplayEventInternal(const FGameplayEventData Payload)
{
	if (GetOwner()->HasAuthority())
	{
		const FGameplayTag EventTag = Payload.EventTag;

		if (EventTag == ProjectNGameplayTags::InventoryTag_Equip)
		{
			if (const UProjectN_ItemInstance* ItemInstance = Cast<UProjectN_ItemInstance>(Payload.OptionalObject))
			{
				AddItemByInstance(const_cast<UProjectN_ItemInstance*>(ItemInstance));

				if (Payload.Instigator)
				{
					//const_cast<AActor*>(Payload->Instigator)->Destroy();
				}
			}
		}
		else if (EventTag == ProjectNGameplayTags::InventoryTag_UnEquip)
		{
			if (const UProjectN_ItemInstance* ItemInstance = Cast<UProjectN_ItemInstance>(Payload.OptionalObject))
			{
				UnEquipItemByInstance(const_cast<UProjectN_ItemInstance*>(ItemInstance));
			}
		}
		else if (EventTag == ProjectNGameplayTags::InventoryTag_Drop)
		{
			if (const UProjectN_ItemInstance* ItemInstance = Cast<UProjectN_ItemInstance>(Payload.OptionalObject))
			{
				DropItem(const_cast<UProjectN_ItemInstance*>(ItemInstance));
			}
		}
	}
}

void UProjectN_InventoryComponent::ServerHandleGameplayEvent_Implementation(const FGameplayEventData Payload)
{
	HandleGameplayEventInternal(Payload);
}

/*void UProjectN_InventoryComponent::EquipTestItem()
{
	if (InventoryList.GetItemsRef().Num() && GetOwner()->HasAuthority())
	{
		//EquipItemByStaticClass(InventoryList.GetItemsRef()[0].ItemInstance->GetItemStaticSubClass());
	}
}*/

/*void UProjectN_InventoryComponent::AddItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItemByStaticClass(ItemStaticDataClass);
	}
}*/

void UProjectN_InventoryComponent::AddItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItemByInstance(InItemInstance);
	}
}

/*void UProjectN_InventoryComponent::RemoveItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItemByStaticClass(ItemStaticDataClass);
	}
}*/

void UProjectN_InventoryComponent::RemoveItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItemByInstance(InItemInstance);
	}
}

/*void UProjectN_InventoryComponent::EquipItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority() && IsValid(Cast<APlayerState>(GetOwner())->GetPawn()))
	{
		// Just check if item has in our inventory
		for (const FInventoryItem& Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance->GetItemStaticSubClass() == ItemStaticDataClass)
			{
				Item.ItemInstance->OnEquip(Cast<APlayerState>(GetOwner())->GetPawn());
                //CurrentItemInstance = Item.ItemInstance;
                break;
			}
		}
	}
}*/

void UProjectN_InventoryComponent::EquipItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	AProjectN_CharacterBase* BaseCharacter = Cast<AProjectN_CharacterBase>(Cast<APlayerState>(GetOwner())->GetPawn());
	
	if (GetOwner()->HasAuthority() && IsValid(BaseCharacter))
	{
		// At first check if item can be equipped
		if (!IsEquippableItem(InItemInstance))
		{
			PrintMessage(TEXT("Item is not equippable"));
			return;
		}

		// Then check if the new item have same slot as equipped
		if (IsSlotEquipped(InItemInstance))
		{
			PrintMessage(TEXT("Slot is equipped"));
			
			FEquippedItemData* FindItemData = FindItemDataBySlot(InItemInstance->GetItemStaticClass()->GetItemSlot());
			
			// If we try to equip already equipped item, don't do anything
			if (FindItemData->ItemInstance == InItemInstance)
			{
				PrintMessage(TEXT("Same Slot"));
				
				return;
			}

			// Else need un equip old item
			UnEquipItemByInstance(FindItemData->ItemInstance);
		}
		
		// Just check if item has in our inventory
		for (const FInventoryItem& Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance == InItemInstance)
			{
				Item.ItemInstance->OnEquip(BaseCharacter);

				// Add item data to list of equipped items
				AddItemToSlot(Item.ItemInstance->GetItemStaticClass()->GetItemSlot(), Item.ItemInstance);

				PrintMessage(TEXT("Slot is equipped and added"));
				break;
			}
		}
	}
}

void UProjectN_InventoryComponent::UnEquipItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		for (const FInventoryItem& Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance == InItemInstance)
			{
				Item.ItemInstance->OnUnEquip();
				
				// Remove item data from list of equipped items
				RemoveSlot(Item.ItemInstance->GetItemStaticClass()->GetItemSlot());
				
				PrintMessage(TEXT("Slot is un equipped and removed"));
				break;
			}
		}
	}
}

void UProjectN_InventoryComponent::DropItem(UProjectN_ItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(CurrentItemInstance))
		{
			InItemInstance->OnDrop();
			//RemoveItemByStaticClass(InItemInstance->GetItemStaticSubClass());
			CurrentItemInstance = nullptr;
		}
	}
}

void UProjectN_InventoryComponent::PrintMessage(const FString& InText)
{
	FVector2D MessageSize = FVector2D(1.f, 1.f);
	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, InText, true, MessageSize);
}

/*********************************
 *  Slots managing
 *********************************/
bool UProjectN_InventoryComponent::IsSlotEquipped(const EItemSlot InItemSlot)
{
	const FEquippedItemData* FindItem = FindItemDataBySlot(InItemSlot);
	return FindItem != nullptr;
}

bool UProjectN_InventoryComponent::IsSlotEquipped(const UProjectN_ItemInstance* InItemInstance)
{
	return IsSlotEquipped(InItemInstance->GetItemStaticClass()->GetItemSlot());
}

FEquippedItemData* UProjectN_InventoryComponent::FindItemDataBySlot(const EItemSlot InItemSlot)
{
	return EquippedItemSlots.FindByPredicate([InItemSlot](const FEquippedItemData& EquippedItemData)
	{
		return EquippedItemData.ItemSlot == InItemSlot;
	});
}

FEquippedItemData* UProjectN_InventoryComponent::FindItemDataByInstance(const UProjectN_ItemInstance* InItemInstance)
{
	return EquippedItemSlots.FindByPredicate([InItemInstance](const FEquippedItemData& EquippedItemData)
	{
		return EquippedItemData.ItemInstance == InItemInstance;
	});
}

void UProjectN_InventoryComponent::RemoveSlot(const EItemSlot InItemSlot)
{
	for (auto ItemIter = EquippedItemSlots.CreateIterator(); ItemIter; ++ItemIter)
	{
		FEquippedItemData& Item = *ItemIter;
		if (Item.ItemSlot == InItemSlot)
		{
			ItemIter.RemoveCurrent();
			//MarkArrayDirty();
			break;
		}
	}
}

void UProjectN_InventoryComponent::AddItemToSlot(const EItemSlot InItemSlot, UProjectN_ItemInstance* InItemInstance)
{
	FEquippedItemData& NewItem = EquippedItemSlots.AddDefaulted_GetRef();
	NewItem.ItemInstance = InItemInstance;
	NewItem.ItemSlot = InItemSlot;
}
/****************************
 ****************************/

/*void UProjectN_InventoryComponent::UnEquipItemByStaticClass(const TSubclassOf<UItemStaticClass> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (const FInventoryItem& Item : InventoryList.GetItemsRef())
		{
			Item.ItemInstance->OnUnEquip();
			CurrentItemInstance = nullptr;
			break;
		}
	}
}*/

bool UProjectN_InventoryComponent::IsEquippableItem(UProjectN_ItemInstance* InItemInstance) const
{
	if (GetOwner()->HasAuthority())
	{
		return InItemInstance->GetItemStaticClass()->CanBeEquipped();
	}
	return false;
}
