// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_CharacterBase.h"
#include "ProjectN_GameplayTags.h"
#include "DataAssets/ProjectN_LootDataAsset.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_WeaponActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

/***************
 * Initialize
 ***************/
UProjectN_InventoryComponent::UProjectN_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UProjectN_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_InventoryComponent, EquippedSlots);
	DOREPLIFETIME(UProjectN_InventoryComponent, BagList);
}

void UProjectN_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		InitBags();
	}
}
/************************************************************************************************
 ************************************************************************************************/



/***************************************
 * Main functions to handle inventory
 ***************************************/
void UProjectN_InventoryComponent::PrintMessage(const FString& InText) const
{
	FVector2D MessageSize = FVector2D(1.f, 1.f);
	//GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, InText, true, MessageSize);
	UKismetSystemLibrary::PrintString(this, InText, true, true, FColor::Blue);
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Getters
 *********************************/
FVector UProjectN_InventoryComponent::FindWeaponSocketLocationForProjectileBySlot(const EItemSlot ItemSlot) const
{
	if (ItemSlot == EItemSlot::None)
	{
		return FVector::ZeroVector;
	}
	
	return Cast<AProjectN_ItemActor_Base>(GetEquippedSlotData(ItemSlot)->SpawnedActor)->GetWeaponSocketLocationForProjectile();
}

AProjectN_WeaponActor* UProjectN_InventoryComponent::GetEquippedWeaponActorBySlot(const EItemSlot InItemSlot)
{
	if (GetEquippedSlotData(InItemSlot))
	{
		return Cast<AProjectN_WeaponActor>(GetEquippedSlotData(InItemSlot)->SpawnedActor);
	}
	
	return nullptr;
}

FGameplayTag UProjectN_InventoryComponent::GetWeaponTypeBySlot(const EItemSlot InItemSlot) const
{
	return GetEquippedWeaponData(InItemSlot)->WeaponTypeTag;
}

TMap<FGameplayTag, float> UProjectN_InventoryComponent::GetWeaponDamageTypesForSlot(const EItemSlot InItemSlot) const
{
	return GetEquippedWeaponData(InItemSlot)->WeaponDamageTypes;
}

bool UProjectN_InventoryComponent::IsSlotEquipped(const EItemSlot Slot)
{
	for (const FEquippedItemData& Item : EquippedSlots.EquippedItems)
	{
		if (Item.ItemSlot == Slot)
		{
			return true;
		}
	}
	return false;
}

const FEquippedItemData* UProjectN_InventoryComponent::GetEquippedSlotData(const FName& ItemID, const EEntryType ItemType) const
{
	for (const FEquippedItemData& Item : EquippedSlots.EquippedItems)
	{
		if (Item.ItemID == ItemID && Item.ItemType == ItemType)
		{
			return &Item;
		}
	}
	return nullptr;
}

const FEquippedItemData* UProjectN_InventoryComponent::GetEquippedSlotData(const EItemSlot ItemSlot) const
{
	for (const FEquippedItemData& Item : EquippedSlots.EquippedItems)
	{
		if (Item.ItemSlot == ItemSlot)
		{
			return &Item;
		}
	}
	return nullptr;
}

const FEquippableItemDefinition* UProjectN_InventoryComponent::GetEquipmentData(const EItemSlot ItemSlot) const
{
	return GetEquippableItemData(GetEquippedSlotData(ItemSlot)->ItemID);
}

const FEquippableItemDefinition* UProjectN_InventoryComponent::GetEquippableItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindEquippableItemFromDataTable"));
	
	if (const FEquippableItemDefinition* WeaponItemDef = EquippableItemDataTable.LoadSynchronous()->FindRow<FEquippableItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FWeaponItemDefinition* UProjectN_InventoryComponent::GetEquippedWeaponData(const EItemSlot ItemSlot) const
{
	return GetWeaponData(GetEquippedSlotData(ItemSlot)->ItemID);
}

const FWeaponItemDefinition* UProjectN_InventoryComponent::GetWeaponData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindWeaponFromDataTable"));
	
	if (const FWeaponItemDefinition* WeaponItemDef = WeaponDataTable.LoadSynchronous()->FindRow<FWeaponItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Bag manage
 *********************************/
void UProjectN_InventoryComponent::InitBags()
{
	for (int32 i = 0; i < BagsDefaultID.Num(); i++)
	{
		AddBag(BagsDefaultID[i]);
	}
}

void UProjectN_InventoryComponent::AddBag(const FName InBagItemID)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindBagFromDataTable"));
		
	if (const FBagDefinition* BagDef = BagDataTable.LoadSynchronous()->FindRow<FBagDefinition>(InBagItemID, Context, false))
	{
		FBagData& NewBag = BagList.Bags.AddDefaulted_GetRef();
		NewBag.BagID = BagList.Bags.Num();
		NewBag.BagItemID = InBagItemID;
		NewBag.Slots.SetNum(BagDef->NumSlots);
		BroadcastBagChange(NewBag.BagID, BagDef->NumSlots);

		BagList.MarkItemDirty(NewBag);
	}
}

void UProjectN_InventoryComponent::RemoveBag(const int32 BagID)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	for (int32 i = 0; i < BagList.Bags.Num(); i++)
	{
		if (BagList.Bags[i].BagID == BagID)
		{
			BagList.Bags.RemoveAt(i);
			BroadcastBagChange(i, 0);
			BagList.MarkArrayDirty();
			return;
		}
	}
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Items manage
 *********************************/
bool UProjectN_InventoryComponent::TryAddItem(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	if (ItemType == EEntryType::Ability || ItemType == EEntryType::None)
	{
		return false;
	}
	
	if (ItemType == EEntryType::Item)
	{
		return TryAddItemToStack(ItemID, ItemType, Quantity);
	}
	for (int32 i = 0; i < Quantity; i++)
	{
		if (!TryAddItemToFirstFreeSlot(ItemID, ItemType, Quantity))
		{
			return false;
		}
	}
	return true;
}

bool UProjectN_InventoryComponent::TryAddItemToFirstFreeSlot(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	for (FBagData& Bag : BagList.Bags)
	{
		for (int32 i = 0; i < Bag.Slots.Num(); i++)
		{
			if (Bag.Slots[i].ItemID.IsNone())
			{
				FInventorySlotData NewSlotData;
				NewSlotData.ItemID = ItemID;
				NewSlotData.EntryType = ItemType;
				NewSlotData.Quantity = Quantity;
				
				Bag.Slots[i] = MoveTemp(NewSlotData);
				BagList.MarkItemDirty(Bag);
				
				BroadcastSlotChange(Bag.BagID, i, Bag.Slots[i]);
				
				return true;
			}
		}
	}
	return false;
}

bool UProjectN_InventoryComponent::TryAddItemToStack(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindItemFromDataTable"));
		
	if (const FItemDefinition* ItemDef = ItemDataTable.LoadSynchronous()->FindRow<FItemDefinition>(ItemID, Context, false))
	{
		const int32 ItemMaxStack = ItemDef->MaxStack;

		for (FBagData& Bag : BagList.Bags)
		{
			for (int32 i = 0; i < Bag.Slots.Num(); i++)
			{
				if (Bag.Slots[i].ItemID == ItemID)
				{
					if (Bag.Slots[i].Quantity != ItemMaxStack)
					{
						const int32 TotalQuantity = Quantity + Bag.Slots[i].Quantity;
						if (TotalQuantity <= ItemMaxStack)
						{
							Bag.Slots[i].Quantity = TotalQuantity;
							
							BroadcastSlotChange(Bag.BagID, i, Bag.Slots[i]);
							
							return true;
						}
						else
						{
							Bag.Slots[i].Quantity = ItemMaxStack;
							const int32 Remaining = ItemMaxStack - TotalQuantity;
							
							BroadcastSlotChange(Bag.BagID, i, Bag.Slots[i]);
							
							return TryAddItemToStack(ItemID, ItemType, Remaining)? true : TryAddItemToFirstFreeSlot(ItemID, ItemType, Remaining);
						}
					}
				}
			}
		}
	}
	return false;
}

void UProjectN_InventoryComponent::RemoveItem(const int32 FromBagID, const int32 FromSlotIndex)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	BagList.Bags[FromBagID].Slots.RemoveAt(FromSlotIndex);

	const FInventorySlotData EmptyInventorySlotData;
	BroadcastSlotChange(FromBagID, FromSlotIndex, EmptyInventorySlotData);
}

void UProjectN_InventoryComponent::ReplaceItemInBag(const int32 FromBagID, const int32 ToBagID, const int32 FromSlotIndex, const int32 ToSlotIndex)
{
	FInventorySlotData FromSlotData;
	FInventorySlotData ToSlotData;

	for (FBagData& Bag : BagList.Bags)
	{
		if (Bag.BagID == FromBagID)
		{
			FromSlotData = Bag.Slots[FromSlotIndex];
		}
		
		if (Bag.BagID == ToBagID)
		{
			ToSlotData = Bag.Slots[ToSlotIndex];
		}
	}

	for (FBagData& Bag : BagList.Bags)
	{
		if (Bag.BagID == FromBagID)
		{
			Bag.Slots[FromSlotIndex] = MoveTemp(ToSlotData);
			BagList.MarkItemDirty(Bag);

			BroadcastSlotChange(Bag.BagID, ToSlotIndex, Bag.Slots[ToSlotIndex]);
		}
		
		if (Bag.BagID == ToBagID)
		{
			Bag.Slots[ToSlotIndex] = MoveTemp(FromSlotData);
			BagList.MarkItemDirty(Bag);

			BroadcastSlotChange(Bag.BagID, ToSlotIndex, Bag.Slots[ToSlotIndex]);
		}
	}
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Equipping manage
 *********************************/
void UProjectN_InventoryComponent::EquipItemToSlot(const FName& ItemID, const EEntryType ItemType, const EItemSlot ToSlot)
{
	AProjectN_CharacterBase* BaseCharacter = Cast<AProjectN_CharacterBase>(Cast<APlayerState>(GetOwner())->GetPawn());
	
	if (GetOwner()->HasAuthority() && IsValid(BaseCharacter))
	{
		// At first check if item can be equipped
		if (ItemType == EEntryType::Item || ItemType == EEntryType::Ability || ItemType == EEntryType::None)
		{
			PrintMessage(TEXT("Item is not equippable"));
			return;
		}

		// If item has allowed slot for equip
		switch (ItemType)
		{
		case EEntryType::Equipment :
			if (const FEquippableItemDefinition* ItemDef = GetEquippableItemData(ItemID))
			{
				if (!ItemDef->AllowedSlots.Contains(ToSlot))
				{
					PrintMessage(TEXT("Not allowed slot for equip"));
					return;
				}
			}
		case EEntryType::Weapon :
			if (const FWeaponItemDefinition* ItemDef = GetWeaponData(ItemID))
			{
				if (!ItemDef->AllowedSlots.Contains(ToSlot))
				{
					PrintMessage(TEXT("Not allowed slot for equip"));
					return;
				}
			}
			default: ;
		}
		
		if (IsSlotEquipped(ToSlot))
		{
			PrintMessage(TEXT("Un equip old slot"));
			// Un equip old item
			UnEquipSlot(ToSlot);
		}

		// Special check for two-handed mode
		if (ToSlot == EItemSlot::TwoHand)
		{
			PrintMessage(TEXT("Un equip slot for two-handed mode"));

			UnEquipSlot(EItemSlot::MainArm);
			UnEquipSlot(EItemSlot::AuxiliaryArm);
		}
		else
		{
			UnEquipSlot(EItemSlot::TwoHand);
		}
		
		
		AActor* ItemActor = SpawnItemActor(ItemID, ItemType, ToSlot, BaseCharacter);

		////////// Add item data to list of equipped items
		FEquippedItemData& NewItem = EquippedSlots.EquippedItems.AddDefaulted_GetRef();
		NewItem.ItemSlot = ToSlot;
		NewItem.ItemType = ItemType;
		NewItem.SpawnedActor = ItemActor;
		NewItem.ItemID = ItemID;
		EquippedSlots.MarkItemDirty(NewItem);
		///////////////////////
		ApplyItemStats(ItemID, ItemType);
		
		if (ItemType == EEntryType::Weapon)
		{
			// Update weapon abilities in accordance with equip mode
			UpdateWeaponMode();
		}
		
		PrintMessage(TEXT("Slot is equipped and added"));
	}
}

AActor* UProjectN_InventoryComponent::SpawnItemActor(const FName& ItemID, const EEntryType ItemType, const EItemSlot EItemSlot, AActor* Owner) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindItemFromDataTable"));

	TSubclassOf<AProjectN_ItemActor_Base> ItemActorSubclass = nullptr;
	FName SocketToAttach = NAME_None;

	switch (ItemType)
	{
	case EEntryType::Equipment :
		if (const FEquippableItemDefinition* EquippableItemDef = GetEquippableItemData(ItemID))
		{
			SocketToAttach = *EquippableItemDef->SocketToAttach.Find(EItemSlot);
			ItemActorSubclass = EquippableItemDef->ItemActorClass;
		}
		
	case EEntryType::Weapon :
		if (const FWeaponItemDefinition* WeaponItemDef = GetWeaponData(ItemID))
		{
			SocketToAttach = *WeaponItemDef->SocketToAttach.Find(EItemSlot);
			ItemActorSubclass = WeaponItemDef->ItemActorClass;
		}
		default : ;
	}

	if (!ItemActorSubclass)
	{
		return nullptr;
	}
	
	if (UWorld* World = Owner->GetWorld())
	{
		const FTransform Transform;
		
		AProjectN_ItemActor_Base* ItemActor = World->SpawnActorDeferred<AProjectN_ItemActor_Base>(ItemActorSubclass, Transform, Owner);
		ItemActor->FinishSpawning(Transform);
		
		if (USkeletalMeshComponent* SkeletalMeshComponent = Cast<ACharacter>(Owner) ? Cast<ACharacter>(Owner)->GetMesh() : nullptr)
		{
			if (SocketToAttach.IsNone())
			{
				return ItemActor;
			}
			ItemActor->AttachToComponent(SkeletalMeshComponent,  FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketToAttach);

			return ItemActor;
		}
	}
	return nullptr;
}

void UProjectN_InventoryComponent::UnEquipSlot(const EItemSlot Slot)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	for (int32 i = 0; i < EquippedSlots.EquippedItems.Num(); i++)
	{
		if (EquippedSlots.EquippedItems[i].ItemSlot == Slot)
		{
			RemoveItemStats(EquippedSlots.EquippedItems[i].ItemID, EquippedSlots.EquippedItems[i].ItemType);
			
			EquippedSlots.EquippedItems[i].SpawnedActor->Destroy();
			EquippedSlots.EquippedItems.RemoveAt(i);
			EquippedSlots.MarkArrayDirty();
			return;
		}
	}
	UpdateWeaponMode();
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Abilities and stats managing
 *********************************/
void UProjectN_InventoryComponent::ApplyItemStats(const FName& ItemID, const EEntryType ItemType) const
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}

	TMap<FGameplayTag, float> ItemBonusAttributes;
	
	switch (ItemType)
	{
	case EEntryType::Equipment :
		if (const FEquippableItemDefinition* EquippableItemDef = GetEquippableItemData(ItemID))
		{
			ItemBonusAttributes = EquippableItemDef->ItemBonusAttributes;
		}
		
	case EEntryType::Weapon :
		if (const FWeaponItemDefinition* WeaponItemDef = GetWeaponData(ItemID))
		{
			ItemBonusAttributes = WeaponItemDef->ItemBonusAttributes;
		}
	default : ;
	}

	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	// Apply item attributes
	for (const TTuple<FGameplayTag, float>& Attribute : ItemBonusAttributes)
	{
		ASC->ServerAddToAttributeByTag(Attribute.Key, Attribute.Value);
	}
}

void UProjectN_InventoryComponent::RemoveItemStats(const FName& ItemID, const EEntryType ItemType) const
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}

	TMap<FGameplayTag, float> ItemBonusAttributes;
	
	switch (ItemType)
	{
	case EEntryType::Equipment :
		if (const FEquippableItemDefinition* EquippableItemDef = GetEquippableItemData(ItemID))
		{
			ItemBonusAttributes = EquippableItemDef->ItemBonusAttributes;
		}
		
	case EEntryType::Weapon :
		if (const FWeaponItemDefinition* WeaponItemDef = GetWeaponData(ItemID))
		{
			ItemBonusAttributes = WeaponItemDef->ItemBonusAttributes;
		}
	default : ;
	}

	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	// Apply item attributes
	for (const TTuple<FGameplayTag, float>& Attribute : ItemBonusAttributes)
	{
		ASC->ServerAddToAttributeByTag(Attribute.Key, -Attribute.Value);
	}
}

void UProjectN_InventoryComponent::GiveWeaponAbilities(const FWeaponItemDefinition* WeaponItemDefinition, const EWeaponMode WeaponMode, const bool bAddForMainHand, const bool bAddForAuxiliaryHand)
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}
	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	
	if (WeaponItemDefinition->WeaponAbilitiesInfo.Find(WeaponMode))
	{
		FGrantedAbilityHandles AbilityHandles;

		if (bAddForMainHand)
		{
			AbilityHandles.AbilitySpecs.Add(ASC->AddAbility(WeaponItemDefinition->WeaponAbilitiesInfo.Find(WeaponMode)->MainWeaponAbility, ProjectNGameplayTags::Input_LMB));
		}
		if (bAddForAuxiliaryHand)
		{
			AbilityHandles.AbilitySpecs.Add(ASC->AddAbility(WeaponItemDefinition->WeaponAbilitiesInfo.Find(WeaponMode)->AuxiliaryWeaponAbility, ProjectNGameplayTags::Input_RMB));
		}
		
		// Save added ability specs to map
		if (GrantedAbilityHandlesByMode.Find(WeaponMode))
		{
			for (const FGameplayAbilitySpecHandle& AbilitySpec : AbilityHandles.AbilitySpecs)
			{
				GrantedAbilityHandlesByMode.Find(WeaponMode)->AbilitySpecs.Add(AbilitySpec);
			}

			return;
		}

		GrantedAbilityHandlesByMode.Add(WeaponMode, AbilityHandles);
	}
}

void UProjectN_InventoryComponent::RemoveWeaponAbilities(const EWeaponMode WeaponMode)
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (GrantedAbilityHandlesByMode.Find(WeaponMode))
	{
		for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : GrantedAbilityHandlesByMode.Find(WeaponMode)->AbilitySpecs)
		{
			ASCInterface->GetAbilitySystemComponent()->ClearAbility(AbilitySpecHandle);
		}
	}
	GrantedAbilityHandlesByMode.Remove(WeaponMode);
}

void UProjectN_InventoryComponent::UpdateWeaponMode()
{
	RemoveWeaponAbilities(EWeaponMode::Single);
	RemoveWeaponAbilities(EWeaponMode::Dual);
	RemoveWeaponAbilities(EWeaponMode::TwoHand);
	
	if (IsSlotEquipped(EItemSlot::TwoHand))
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EItemSlot::TwoHand), EWeaponMode::TwoHand, true, true);

		return;
	}
	if (IsSlotEquipped(EItemSlot::MainArm) && IsSlotEquipped(EItemSlot::AuxiliaryArm) &&  GetEquippedWeaponData(EItemSlot::MainArm)->WeaponTypeTag == GetEquippedWeaponData(EItemSlot::AuxiliaryArm)->WeaponTypeTag)
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EItemSlot::MainArm), EWeaponMode::Dual, true, true);

		return;
	}
	if (IsSlotEquipped(EItemSlot::MainArm))
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EItemSlot::MainArm), EWeaponMode::Single, true, false);
	}
	if (IsSlotEquipped(EItemSlot::AuxiliaryArm))
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EItemSlot::AuxiliaryArm), EWeaponMode::Single, false, true);
	}
}
/************************************************************************************************
 ************************************************************************************************/


/***********************************
 *  Broadcast to widget controller
 ***********************************/
void UProjectN_InventoryComponent::BroadcastBagChange_Implementation(const int32 BagID, const int32 BagSlots)
{
	if (OnBagChanged.IsBound())
	{
		OnBagChanged.Execute(BagID, BagSlots);
	}
}

void UProjectN_InventoryComponent::BroadcastSlotChange_Implementation(const int32 BagID, const int32 BagSlot, const FInventorySlotData& ItemData)
{
	if (OnSlotChange.IsBound())
	{
		OnSlotChange.Execute(BagID, BagSlot, ItemData);
	}
}