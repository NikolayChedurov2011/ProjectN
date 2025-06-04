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
		// TODO: Load Items
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
FVector UProjectN_InventoryComponent::FindWeaponSocketLocationForProjectileBySlot(const EEquipSlot ItemSlot) const
{
	if (ItemSlot == EEquipSlot::None)
	{
		return FVector::ZeroVector;
	}
	
	return Cast<AProjectN_ItemActor_Base>(GetEquippedSlotData(ItemSlot)->SpawnedActor)->GetWeaponSocketLocationForProjectile();
}

AProjectN_WeaponActor* UProjectN_InventoryComponent::GetEquippedWeaponActorBySlot(const EEquipSlot InItemSlot)
{
	if (GetEquippedSlotData(InItemSlot))
	{
		return Cast<AProjectN_WeaponActor>(GetEquippedSlotData(InItemSlot)->SpawnedActor);
	}
	
	return nullptr;
}

FGameplayTag UProjectN_InventoryComponent::GetWeaponTypeBySlot(const EEquipSlot InItemSlot) const
{
	return GetEquippedWeaponData(InItemSlot)->WeaponTypeTag;
}

TMap<FGameplayTag, float> UProjectN_InventoryComponent::GetWeaponDamageTypesForSlot(const EEquipSlot InItemSlot) const
{
	return GetEquippedWeaponData(InItemSlot)->WeaponDamageTypes;
}

bool UProjectN_InventoryComponent::IsSlotEquipped(const EEquipSlot Slot)
{
	for (const FEquipSlotData& Item : EquippedSlots.EquippedItems)
	{
		if (Item.EquipSlot == Slot)
		{
			return true;
		}
	}
	return false;
}

const FEquipSlotData* UProjectN_InventoryComponent::GetEquippedSlotData(const FName& ItemID, const EEntryType ItemType) const
{
	for (const FEquipSlotData& Item : EquippedSlots.EquippedItems)
	{
		if (Item.ItemID == ItemID && Item.ItemType == ItemType)
		{
			return &Item;
		}
	}
	return nullptr;
}

const FEquipSlotData* UProjectN_InventoryComponent::GetEquippedSlotData(const EEquipSlot ItemSlot) const
{
	for (const FEquipSlotData& Item : EquippedSlots.EquippedItems)
	{
		if (Item.EquipSlot == ItemSlot)
		{
			return &Item;
		}
	}
	return nullptr;
}

const FEquippableItemDefinition* UProjectN_InventoryComponent::GetEquipmentData(const EEquipSlot ItemSlot) const
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

const FWeaponItemDefinition* UProjectN_InventoryComponent::GetEquippedWeaponData(const EEquipSlot ItemSlot) const
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
		NewBag.BagIndex = BagList.Bags.Num();
		NewBag.BagItemID = InBagItemID;
		NewBag.Slots.SetNum(BagDef->NumSlots);

		// Init slots
		for (int32 i = 0; i < NewBag.Slots.Num(); i++)
		{
			NewBag.Slots[i].BagIndex = NewBag.BagIndex;
			NewBag.Slots[i].SlotIndex = i;
		}
		
		BroadcastBagChange(NewBag);

		BagList.MarkItemDirty(NewBag);
	}
}

void UProjectN_InventoryComponent::RemoveBag(const int32 BagIndex)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	for (int32 i = 0; i < BagList.Bags.Num(); i++)
	{
		if (BagList.Bags[i].BagIndex == BagIndex)
		{
			BagList.Bags.RemoveAt(i);

			FBagData EmptyBag;
			EmptyBag.BagIndex = BagIndex;
			
			BroadcastBagChange(EmptyBag);
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
void UProjectN_InventoryComponent::TryAddItem_Implementation(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	if (ItemType == EEntryType::Ability || ItemType == EEntryType::None)
	{
		return;
	}
	
	if (ItemType == EEntryType::Item)
	{
		TryAddItemToStack(ItemID, ItemType, Quantity);
	}
	for (int32 i = 0; i < Quantity; i++)
	{
		if (!TryAddItemToFirstFreeSlot(ItemID, ItemType, Quantity))
		{
			return;
		}
	}
	return;
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
				NewSlotData.ItemType = ItemType;
				NewSlotData.Quantity = Quantity;
				NewSlotData.BagIndex = Bag.BagIndex;
				NewSlotData.SlotIndex = i;
				
				Bag.Slots[i] = MoveTemp(NewSlotData);
				BagList.MarkItemDirty(Bag);
				
				BroadcastSlotChange(Bag.Slots[i]);
				
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
							
							BroadcastSlotChange(Bag.Slots[i]);
							
							return true;
						}
						else
						{
							Bag.Slots[i].Quantity = ItemMaxStack;
							const int32 Remaining = ItemMaxStack - TotalQuantity;
							
							BroadcastSlotChange(Bag.Slots[i]);
							
							return TryAddItemToStack(ItemID, ItemType, Remaining)? true : TryAddItemToFirstFreeSlot(ItemID, ItemType, Remaining);
						}
					}
				}
			}
		}
	}
	return false;
}

void UProjectN_InventoryComponent::RemoveItem_Implementation(const int32 FromBagIndex, const int32 FromSlotIndex)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	for (FBagData& Bag : BagList.Bags)
	{
		if (Bag.BagIndex == FromBagIndex)
		{
			for (int32 i = 0; i < Bag.Slots.Num(); i++)
			{
				if (Bag.Slots[i].SlotIndex == FromSlotIndex)
				{
					Bag.Slots[i].ItemID = NAME_None;
					Bag.Slots[i].ItemType = EEntryType::None;
					Bag.Slots[i].ItemIcon = nullptr;
					Bag.Slots[i].Quantity = 0;
					BagList.MarkItemDirty(Bag);
					
					BroadcastSlotChange(Bag.Slots[i]);

					return;
				}
			}
		}
	}	
}

void UProjectN_InventoryComponent::ReplaceItemInBag_Implementation(const int32 FromBagIndex, const int32 ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex)
{
	FInventorySlotData FromSlotData;
	FInventorySlotData ToSlotData;

	for (FBagData& Bag : BagList.Bags)
	{
		if (Bag.BagIndex == FromBagIndex)
		{
			FromSlotData = Bag.Slots[FromSlotIndex];
		}
		
		if (Bag.BagIndex == ToBagIndex)
		{
			ToSlotData = Bag.Slots[ToSlotIndex];
		}
	}

	for (FBagData& Bag : BagList.Bags)
	{
		if (Bag.BagIndex == FromBagIndex)
		{
			Bag.Slots[FromSlotIndex].ItemID = ToSlotData.ItemID;
			Bag.Slots[FromSlotIndex].ItemType = ToSlotData.ItemType;
			Bag.Slots[FromSlotIndex].ItemIcon = ToSlotData.ItemIcon;
			Bag.Slots[FromSlotIndex].Quantity = ToSlotData.Quantity;
			BagList.MarkItemDirty(Bag);

			BroadcastSlotChange(Bag.Slots[FromSlotIndex]);
		}
		
		if (Bag.BagIndex == ToBagIndex)
		{
			Bag.Slots[ToSlotIndex].ItemID = FromSlotData.ItemID;
			Bag.Slots[ToSlotIndex].ItemType = FromSlotData.ItemType;
			Bag.Slots[ToSlotIndex].ItemIcon = FromSlotData.ItemIcon;
			Bag.Slots[ToSlotIndex].Quantity = FromSlotData.Quantity;
			BagList.MarkItemDirty(Bag);

			BroadcastSlotChange(Bag.Slots[ToSlotIndex]);
		}
	}
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Equipping manage
 *********************************/
void UProjectN_InventoryComponent::EquipItemToSlot_Implementation(const FName& ItemID, const EEntryType ItemType, const EEquipSlot ToSlot)
{
	AProjectN_CharacterBase* BaseCharacter = Cast<AProjectN_CharacterBase>(Cast<APlayerState>(GetOwner())->GetPawn());
	
	if (GetOwner()->HasAuthority() && IsValid(BaseCharacter))
	{
		// At first check if item can be equipped
		if (ItemType == EEntryType::Item || ItemType == EEntryType::Ability || ItemType == EEntryType::None)
		{
			PrintMessage(TEXT("Item is not equippable"));
			TryAddItem(ItemID, ItemType, 1);
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
					TryAddItem(ItemID, ItemType, 1);
					return;
				}
			}
		case EEntryType::Weapon :
			if (const FWeaponItemDefinition* ItemDef = GetWeaponData(ItemID))
			{
				if (!ItemDef->AllowedSlots.Contains(ToSlot))
				{
					PrintMessage(TEXT("Not allowed slot for equip"));
					TryAddItem(ItemID, ItemType, 1);
					return;
				}
			}
			default: ;
		}
		
		if (IsSlotEquipped(ToSlot))
		{
			PrintMessage(TEXT("Un equip old slot"));
			// Un equip old item
			UnEquipSlotAndReturnWeapon(ToSlot);
		}

		// Special check for two-handed mode
		if (ToSlot == EEquipSlot::TwoHand)
		{
			PrintMessage(TEXT("Un equip slot for two-handed mode"));

			UnEquipSlotAndReturnWeapon(EEquipSlot::MainArm);
			UnEquipSlotAndReturnWeapon(EEquipSlot::AuxiliaryArm);
		}
		else
		{
			UnEquipSlotAndReturnWeapon(EEquipSlot::TwoHand);
		}
		
		
		AActor* ItemActor = SpawnItemActor(ItemID, ItemType, ToSlot, BaseCharacter);

		////////// Add item data to list of equipped items
		FEquipSlotData& NewItem = EquippedSlots.EquippedItems.AddDefaulted_GetRef();
		NewItem.EquipSlot = ToSlot;
		NewItem.ItemType = ItemType;
		NewItem.SpawnedActor = ItemActor;
		NewItem.ItemID = ItemID;
		EquippedSlots.MarkItemDirty(NewItem);

		BroadcastEquipSlotChange(NewItem);
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

AActor* UProjectN_InventoryComponent::SpawnItemActor(const FName& ItemID, const EEntryType ItemType, const EEquipSlot EItemSlot, AActor* Owner) const
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

void UProjectN_InventoryComponent::UnEquipSlot_Implementation(const EEquipSlot Slot)
{
	for (int32 i = 0; i < EquippedSlots.EquippedItems.Num(); i++)
	{
		if (EquippedSlots.EquippedItems[i].EquipSlot == Slot)
		{
			RemoveItemStats(EquippedSlots.EquippedItems[i].ItemID, EquippedSlots.EquippedItems[i].ItemType);
			
			EquippedSlots.EquippedItems[i].SpawnedActor->Destroy();
			EquippedSlots.EquippedItems.RemoveAt(i);
			EquippedSlots.MarkArrayDirty();

			FEquipSlotData EmptyEquipSlot;
			EmptyEquipSlot.EquipSlot = Slot;
			BroadcastEquipSlotChange(EmptyEquipSlot);
			break;
		}
	}
	UpdateWeaponMode();
}

void UProjectN_InventoryComponent::UnEquipSlotAndReturnWeapon(const EEquipSlot Slot)
{
	for (int32 i = 0; i < EquippedSlots.EquippedItems.Num(); i++)
	{
		if (EquippedSlots.EquippedItems[i].EquipSlot == Slot)
		{
			TryAddItem(EquippedSlots.EquippedItems[i].ItemID, EquippedSlots.EquippedItems[i].ItemType, 1);
			UnEquipSlot(Slot);

			return;
		}
	}
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
	
	if (IsSlotEquipped(EEquipSlot::TwoHand))
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EEquipSlot::TwoHand), EWeaponMode::TwoHand, true, true);

		return;
	}
	if (IsSlotEquipped(EEquipSlot::MainArm) && IsSlotEquipped(EEquipSlot::AuxiliaryArm) &&  GetEquippedWeaponData(EEquipSlot::MainArm)->WeaponTypeTag == GetEquippedWeaponData(EEquipSlot::AuxiliaryArm)->WeaponTypeTag)
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EEquipSlot::MainArm), EWeaponMode::Dual, true, true);

		return;
	}
	if (IsSlotEquipped(EEquipSlot::MainArm))
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EEquipSlot::MainArm), EWeaponMode::Single, true, false);
	}
	if (IsSlotEquipped(EEquipSlot::AuxiliaryArm))
	{
		GiveWeaponAbilities(GetEquippedWeaponData(EEquipSlot::AuxiliaryArm), EWeaponMode::Single, false, true);
	}
}
/************************************************************************************************
 ************************************************************************************************/


/***********************************
 *  Broadcast to widget controller
 ***********************************/
void UProjectN_InventoryComponent::BroadcastBagChange_Implementation(const FBagData& BagData)
{
	if (OnBagChanged.IsBound())
	{
		OnBagChanged.Execute(BagData);
	}
}

void UProjectN_InventoryComponent::BroadcastSlotChange_Implementation(const FInventorySlotData& ItemData)
{
	if (OnInventorySlotChange.IsBound())
	{
		OnInventorySlotChange.Execute(ItemData);
	}
}

void UProjectN_InventoryComponent::BroadcastEquipSlotChange_Implementation(const FEquipSlotData& EquipSlotData)
{
	if (OnEquipSlotChange.IsBound())
	{
		OnEquipSlotChange.Execute(EquipSlotData);
	}
}
