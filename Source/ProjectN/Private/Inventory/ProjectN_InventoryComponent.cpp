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
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UProjectN_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_InventoryComponent, EquippedSlots);
	DOREPLIFETIME(UProjectN_InventoryComponent, BagList);
	DOREPLIFETIME(UProjectN_InventoryComponent, CurrentTwoHandedPosture);
	DOREPLIFETIME(UProjectN_InventoryComponent, bIsTwoHandedEquip);
}

void UProjectN_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		
	}

	BagList.InventoryComponent = this;
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
FInventorySlotData* UProjectN_InventoryComponent::GetBagSlot(const FGuid BagIndex, const int32 SlotIndex)
{
	for (FInventorySlotData& BagSlot : BagList.Slots)
	{
		if (BagSlot.BagIndex == BagIndex && BagSlot.SlotIndex == SlotIndex)
		{
			return &BagSlot;
		}
	}
	return nullptr;
}

FInventorySlotData* UProjectN_InventoryComponent::FindItemInBag(const FName ItemID)
{
	for (FInventorySlotData& BagSlot : BagList.Slots)
	{
		if (BagSlot.ItemID == ItemID)
		{
			return &BagSlot;
		}
	}
	return nullptr;
}

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
	return GetEquippedWeaponFragment(InItemSlot)->GetWeaponTypeTag();
}

TMap<FGameplayTag, float> UProjectN_InventoryComponent::GetWeaponDamageTypesForSlot(const EEquipSlot InItemSlot) const
{
	TInstancedStruct<FLabeledFragment> DamageTypes = GetEquippedWeaponFragment(InItemSlot)->GetWeaponDamageTypes();
	
	TMap<FGameplayTag, float> ReturnDamageTypes;
	ReturnDamageTypes.Add(DamageTypes.Get<FLabeledFragment>().GetTag(), DamageTypes.Get<FLabeledFragment>().GetValue());
	
	return ReturnDamageTypes;
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

const FWeaponFragment* UProjectN_InventoryComponent::GetEquippedWeaponFragment(const EEquipSlot ItemSlot) const
{
	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(GetEquippedSlotData(ItemSlot)->ItemID);
	
	if (!EntriesDefinition)
	{
		return nullptr;
	}

	const FWeaponFragment* WeaponFragment = GetFragment<FWeaponFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Weapon);

	if (!WeaponFragment)
	{
		return nullptr;
	}
	
	return WeaponFragment;
}

FEntriesDefinition* UProjectN_InventoryComponent::GetEntryManifest(const FName& ItemID) const
{
	const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindEntryFromDataTable"));

	if (!Entries) return nullptr;
		
	/*if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(ItemID, Context, false))
	{
		return EntriesDefinition;
	}*/
	if (FEntriesDefinition* EntriesDefinition = Entries->FindRow<FEntriesDefinition>(ItemID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Bag manage
 *********************************/
void UProjectN_InventoryComponent::ServerInitBags_Implementation()
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

	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(InBagItemID);
	
	if (!EntriesDefinition)
	{
		return;
	}
	
	const FBagFragment* BagFragment = GetFragment<FBagFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Bag);
	
	if (BagFragment)
	{
		const FGuid NewBagIndex = FGuid::NewGuid();

		// Init slots
		for (int32 i = 0; i < BagFragment->GetNumSlots(); i++)
		{
			FInventorySlotData Slot;
			Slot.BagIndex = NewBagIndex;
			Slot.SlotIndex = i;

			BagList.Slots.Add(MoveTemp(Slot));
		}
		BagList.MarkArrayDirty();
		
		BroadcastBagChange(BagList.Slots[BagList.Slots.Num() - 1].BagIndex, BagFragment->GetNumSlots());
	}
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Items manage
 *********************************/
void UProjectN_InventoryComponent::ServerTryAddItem_Implementation(const FName& ItemID, const int32 Quantity)
{
	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);

	if (!EntriesDefinition)
	{
		return;
	}
	
	const FTypeFragment* TypeFragment = GetFragment<FTypeFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Type);

	if (!TypeFragment)
	{
		return;
	}

	if (TypeFragment->GetEntryType() == EEntryType::Ability || TypeFragment->GetEntryType() == EEntryType::None)
	{
		return;
	}
	
	const FStackFragment* StackFragment = GetFragment<FStackFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Stack);

	if (StackFragment->GetMaxStack() > 1)
	{
		TryAddItemToStack(ItemID, Quantity, StackFragment->GetMaxStack());
	}
	else
	{
		for (int32 i = 0; i < Quantity; i++)
		{
			if (!TryAddItemToFirstFreeSlot(ItemID, 1))
			{
				// TODO: No place notification
				return;
			}
		}
	}
}

bool UProjectN_InventoryComponent::TryAddItemToFirstFreeSlot(const FName& ItemID, const int32 Quantity)
{
	for (FInventorySlotData& Slot : BagList.Slots)
	{
		if (Slot.ItemID.IsNone())
		{
			Slot.ItemID = ItemID;
			Slot.Quantity = Quantity;
			
			BagList.MarkItemDirty(Slot);
			
			return true;
		}
	}
	
	return false;
}

void UProjectN_InventoryComponent::ServerTryAddItemToSlot_Implementation(const FGuid BagIndex, const int32 SlotIndex, const FName& ItemID, const int32 Quantity)
{
	if (!GetBagSlot(BagIndex, SlotIndex))
	{
		return;
	}
	
	FInventorySlotData& ToSlot = *GetBagSlot(BagIndex, SlotIndex);

	ToSlot.ItemID = ItemID;
	ToSlot.Quantity = Quantity;

	BagList.MarkItemDirty(ToSlot);
}

void UProjectN_InventoryComponent::TryAddItemToStack(const FName& ItemID, const int32 Quantity, const int32 MaxStack)
{
	if (FInventorySlotData* Item = FindItemInBag(ItemID))
	{
		if (Item->Quantity != MaxStack)
		{
			const int32 TotalQuantity = Quantity + Item->Quantity;
			if (TotalQuantity <= MaxStack)
			{
				Item->Quantity = TotalQuantity;
			
				BagList.MarkItemDirty(*GetBagSlot(Item->BagIndex, Item->SlotIndex));
				return;
			}
			else
			{
				Item->Quantity = MaxStack;
				BagList.MarkItemDirty(*GetBagSlot(Item->BagIndex, Item->SlotIndex));
			
				const int32 Remaining = TotalQuantity - MaxStack;
			
				TryAddItemToFirstFreeSlot(ItemID, Remaining);
				return;
			}
		}
	}
	
	// If such item not exist in the bag
	if (MaxStack >= Quantity)
	{
		TryAddItemToFirstFreeSlot(ItemID, Quantity);
	}
	else
	{
		const int32 Remaining = Quantity - MaxStack;

		if (TryAddItemToFirstFreeSlot(ItemID, MaxStack))
		{
			TryAddItemToStack(ItemID, Remaining, MaxStack);
		}
		else
		{
			return;
		}
	}
}

void UProjectN_InventoryComponent::ServerStackItems_Implementation(const FGuid FromBagIndex, const FGuid ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex, const int32 QuantityToAdd, const int32 MaxStack)
{
	if (!GetBagSlot(ToBagIndex, ToSlotIndex) || !GetBagSlot(FromBagIndex, FromSlotIndex))
	{
		return;
	}
	
	FInventorySlotData& ToSlot = *GetBagSlot(ToBagIndex, ToSlotIndex);
	
	if (MaxStack == ToSlot.Quantity || MaxStack == QuantityToAdd)
	{
		ServerTryAddItemToSlot(FromBagIndex, FromSlotIndex, ToSlot.ItemID, ToSlot.Quantity);
		
		ToSlot.Quantity = QuantityToAdd;

		BagList.MarkItemDirty(ToSlot);		
		return;
	}
	if (MaxStack >= ToSlot.Quantity + QuantityToAdd)
	{
		ToSlot.Quantity += QuantityToAdd;
		BagList.MarkItemDirty(ToSlot);
		return;
	}
	else
	{
		const int32 Remaining = QuantityToAdd - (MaxStack - ToSlot.Quantity);
		ToSlot.Quantity = MaxStack;
		BagList.MarkItemDirty(ToSlot);

		ServerTryAddItemToSlot(FromBagIndex, FromSlotIndex, ToSlot.ItemID, Remaining);
	}
}


void UProjectN_InventoryComponent::ServerRemoveItem_Implementation(const FGuid FromBagIndex, const int32 FromSlotIndex)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	if (!GetBagSlot(FromBagIndex, FromSlotIndex))
	{
		return;
	}
	
	FInventorySlotData& FromSlot = *GetBagSlot(FromBagIndex, FromSlotIndex);
	
	FromSlot.ItemID = NAME_None;
	FromSlot.Quantity = 0;
	
	BagList.MarkItemDirty(FromSlot);
}

void UProjectN_InventoryComponent::ServerReplaceItemInBag_Implementation(const FGuid FromBagIndex, const FGuid ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex)
{
	if (!GetBagSlot(ToBagIndex, ToSlotIndex) || !GetBagSlot(FromBagIndex, FromSlotIndex))
	{
		return;
	}
	
	FInventorySlotData& ToSlotData = *GetBagSlot(ToBagIndex, ToSlotIndex);
	FInventorySlotData& FromSlotData = *GetBagSlot(FromBagIndex, FromSlotIndex);
	const FInventorySlotData ToSlotDataCopy = *GetBagSlot(ToBagIndex, ToSlotIndex);
	const FInventorySlotData FromSlotDataCopy = *GetBagSlot(FromBagIndex, FromSlotIndex);

	ToSlotData.ItemID = FromSlotDataCopy.ItemID;
	ToSlotData.Quantity = FromSlotDataCopy.Quantity;

	FromSlotData.ItemID = ToSlotDataCopy.ItemID;
	FromSlotData.Quantity = ToSlotDataCopy.Quantity;
	
	BagList.MarkItemDirty(ToSlotData);
	BagList.MarkItemDirty(FromSlotData);
}

void UProjectN_InventoryComponent::ServerTryUseItem_Implementation(const FName& ItemID)
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);

	if (!EntriesDefinition || !FindItemInBag(ItemID))
	{
		return;
	}
	
	const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
	const FConsumableFragment* ConsumableFragment = GetFragment<FConsumableFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Consumable);

	if (!AbilityFragment || !ConsumableFragment)
	{
		return;
	}

	bool bSuccess = false;
	if (IsValid(AbilityFragment->GetAbilityClass()))
	{
		bSuccess = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent())->TryActivateActionBarAbility(AbilityFragment->GetAbilityClass(), AbilityFragment->GetCooldownTag(), ItemID);
	}
	
	if (ConsumableFragment->IsShouldDestroyAfterUse() && bSuccess)
	{
		FInventorySlotData* InventorySlot = FindItemInBag(ItemID);
		InventorySlot->Quantity--;
		if (!InventorySlot->Quantity)
		{
			ServerRemoveItem(InventorySlot->BagIndex, InventorySlot->SlotIndex);
		}
	}
	
}
/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Equipping manage
 *********************************/
void UProjectN_InventoryComponent::ServerEquipItemToSlot_Implementation(const FName& ItemID, const EEquipSlot ToSlot, const int32 ItemStack)
{
	AProjectN_CharacterBase* BaseCharacter = Cast<AProjectN_CharacterBase>(Cast<APlayerState>(GetOwner())->GetPawn());

	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);
	
	if (!GetOwner()->HasAuthority() || !IsValid(BaseCharacter) && !EntriesDefinition)
	{
		return;
	}

	const FTypeFragment* TypeFragment = GetFragment<FTypeFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Type);
	const FEquippingFragment* EquippingFragment = GetFragment<FEquippingFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Equipment);

	if (!TypeFragment || !EquippingFragment)
	{
		// Just return item back to bag
		ServerTryAddItem(ItemID, ItemStack);
		return;
	}
	
	// At first check if item can be equipped
	if (TypeFragment->GetEntryType() == EEntryType::Item || TypeFragment->GetEntryType() == EEntryType::Ability || TypeFragment->GetEntryType() == EEntryType::None)
	{
		// Just return item back to bag
		ServerTryAddItem(ItemID, ItemStack);
		return;
	}

	// Check if item has allowed slot
	if (!EquippingFragment->GetAllowedSlots().Contains(ToSlot))
	{
		// Just return item back to bag
		ServerTryAddItem(ItemID, ItemStack);
		return;
	}

	// Check if slot already equipped
	if (IsSlotEquipped(ToSlot))
	{
		// Un equip old item
		UnEquipSlotAndReturnWeapon(ToSlot);
	}

	// Special check for two-handed mode
	if (ToSlot == EEquipSlot::TwoHand)
	{
		UnEquipSlotAndReturnWeapon(EEquipSlot::MainArm);
		UnEquipSlotAndReturnWeapon(EEquipSlot::AuxiliaryArm);

		UseNewTwoHandedPosture(ItemID);
		bIsTwoHandedEquip = true;
	}
	else
	{
		UnEquipSlotAndReturnWeapon(EEquipSlot::TwoHand);

		ResetCurrentTwoHandedPosture();
		bIsTwoHandedEquip = false;
	}
	
	AActor* ItemActor = SpawnItemActor(*EquippingFragment, ToSlot, BaseCharacter);

	////////// Add item data to list of equipped items
	FEquipSlotData& NewItem = EquippedSlots.EquippedItems.AddDefaulted_GetRef();
	NewItem.EquipSlot = ToSlot;
	NewItem.SpawnedActor = ItemActor;
	NewItem.ItemID = ItemID;
	EquippedSlots.MarkItemDirty(NewItem);

	BroadcastEquipSlotChange(NewItem);
	///////////////////////
	ApplyItemStats(*EquippingFragment);
	
	if (TypeFragment->GetEntryType() == EEntryType::Weapon)
	{
		// Update weapon abilities in accordance with equip mode
		UpdateWeaponMode();
	}
}

AActor* UProjectN_InventoryComponent::SpawnItemActor(const FEquippingFragment& EquippingFragment, const EEquipSlot EItemSlot, AActor* Owner) const
{
	const TSubclassOf<AProjectN_ItemActor_Base> ItemActorSubclass = EquippingFragment.GetItemActorClass();
	const FName SocketToAttach = *EquippingFragment.GetSocketToAttach().Find(EItemSlot);

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

void UProjectN_InventoryComponent::UseNewTwoHandedPosture(const FName& ItemID)
{
	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);

	if (!EntriesDefinition)
	{
		return;
	}
	
	const FWeaponFragment* WeaponFragment = GetFragment<FWeaponFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Weapon);

	if (!WeaponFragment)
	{
		return;
	}
	
	SetCurrentTwoHandedPosture(WeaponFragment->GetTwoHandedPosture());
}

void UProjectN_InventoryComponent::ResetCurrentTwoHandedPosture()
{
	SetCurrentTwoHandedPosture(nullptr);
}

void UProjectN_InventoryComponent::ServerUnEquipSlot_Implementation(const EEquipSlot Slot)
{
	for (int32 i = 0; i < EquippedSlots.EquippedItems.Num(); i++)
	{
		if (EquippedSlots.EquippedItems[i].EquipSlot == Slot)
		{
			RemoveItemStats(EquippedSlots.EquippedItems[i].ItemID);
			
			EquippedSlots.EquippedItems[i].SpawnedActor->Destroy();
			EquippedSlots.EquippedItems.RemoveAt(i);
			EquippedSlots.MarkArrayDirty();

			FEquipSlotData EmptyEquipSlot;
			EmptyEquipSlot.EquipSlot = Slot;
			BroadcastEquipSlotRemoved(EmptyEquipSlot);
			break;
		}
	}
	UpdateWeaponMode();

	if (Slot == EEquipSlot::TwoHand)
	{
		ResetCurrentTwoHandedPosture();
	}
}

void UProjectN_InventoryComponent::UnEquipSlotAndReturnWeapon(const EEquipSlot Slot)
{
	for (int32 i = 0; i < EquippedSlots.EquippedItems.Num(); i++)
	{
		if (EquippedSlots.EquippedItems[i].EquipSlot == Slot)
		{
			ServerTryAddItem(EquippedSlots.EquippedItems[i].ItemID, 1);
			ServerUnEquipSlot(Slot);

			return;
		}
	}
}

/************************************************************************************************
 ************************************************************************************************/



/*********************************
 *  Abilities and stats managing
 *********************************/
void UProjectN_InventoryComponent::ApplyItemStats(const FEquippingFragment& EquippingFragment) const
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}

	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	// Apply item attributes
	if (EquippingFragment.GetItemBonusAttributes().IsValid())
	{
		for (const auto& Attribute : EquippingFragment.GetItemBonusAttributes().Get<FModifierFragment>().GetModifiers())
		{
			ASC->ServerAddToAttributeByTag(Attribute.Get<FLabeledFragment>().GetTag(), Attribute.Get<FLabeledFragment>().GetValue());
		}
	}
}

void UProjectN_InventoryComponent::RemoveItemStats(const FName& ItemID) const
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}

	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);
	
	if (!EntriesDefinition)
	{
		return;
	}

	const FEquippingFragment* EquippingFragment = GetFragment<FEquippingFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Equipment);

	if (!EquippingFragment)
	{
		return;
	}

	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	// Apply item attributes
	if (EquippingFragment->GetItemBonusAttributes().IsValid())
	{
		for (const auto& Attribute : EquippingFragment->GetItemBonusAttributes().Get<FModifierFragment>().GetModifiers())
		{
			ASC->ServerAddToAttributeByTag(Attribute.Get<FLabeledFragment>().GetTag(), -Attribute.Get<FLabeledFragment>().GetValue());
		}
	}
}

void UProjectN_InventoryComponent::GiveWeaponAbilities(const FWeaponFragment* WeaponFragment, const EWeaponMode WeaponMode, const bool bAddForMainHand, const bool bAddForAuxiliaryHand)
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}
	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	
	if (WeaponFragment->GetWeaponAbilitiesInfo().Find(WeaponMode))
	{
		FGrantedAbilityHandles AbilityHandles;

		if (bAddForMainHand)
		{
			AbilityHandles.AbilitySpecs.Add(ASC->AddAbility(WeaponFragment->GetWeaponAbilitiesInfo().Find(WeaponMode)->MainWeaponAbility, ProjectNGameplayTags::Input_LMB));
		}
		if (bAddForAuxiliaryHand)
		{
			AbilityHandles.AbilitySpecs.Add(ASC->AddAbility(WeaponFragment->GetWeaponAbilitiesInfo().Find(WeaponMode)->AuxiliaryWeaponAbility, ProjectNGameplayTags::Input_RMB));
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
		GiveWeaponAbilities(GetEquippedWeaponFragment(EEquipSlot::TwoHand), EWeaponMode::TwoHand, true, true);

		return;
	}
	if (IsSlotEquipped(EEquipSlot::MainArm) && IsSlotEquipped(EEquipSlot::AuxiliaryArm) &&  GetEquippedWeaponFragment(EEquipSlot::MainArm)->GetWeaponTypeTag() == GetEquippedWeaponFragment(EEquipSlot::AuxiliaryArm)->GetWeaponTypeTag())
	{
		GiveWeaponAbilities(GetEquippedWeaponFragment(EEquipSlot::MainArm), EWeaponMode::Dual, true, true);

		return;
	}
	if (IsSlotEquipped(EEquipSlot::MainArm))
	{
		GiveWeaponAbilities(GetEquippedWeaponFragment(EEquipSlot::MainArm), EWeaponMode::Single, true, false);
	}
	if (IsSlotEquipped(EEquipSlot::AuxiliaryArm))
	{
		GiveWeaponAbilities(GetEquippedWeaponFragment(EEquipSlot::AuxiliaryArm), EWeaponMode::Single, false, true);
	}
}
/************************************************************************************************
 ************************************************************************************************/


/***********************************
 *  Broadcast to widget controller
 ***********************************/
void UProjectN_InventoryComponent::BroadcastBagChange_Implementation(const FGuid BagIndex, const int32 SlotsNum)
{
	if (OnBagChanged.IsBound())
	{
		OnBagChanged.Execute(BagIndex, SlotsNum);
	}
}

void UProjectN_InventoryComponent::BroadcastEquipSlotChange_Implementation(const FEquipSlotData& EquipSlotData)
{
	if (OnEquipSlotChange.IsBound())
	{
		OnEquipSlotChange.Execute(EquipSlotData);
	}
}

void UProjectN_InventoryComponent::BroadcastEquipSlotRemoved_Implementation(const FEquipSlotData& EquipSlotData)
{
	if (OnEquipSlotRemoved.IsBound())
	{
		OnEquipSlotRemoved.Execute(EquipSlotData);
	}
}