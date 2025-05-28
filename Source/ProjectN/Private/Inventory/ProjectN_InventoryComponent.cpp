// N Chedurov All Rights Reserved


#include "Inventory/ProjectN_InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectN_CharacterBase.h"
#include "ProjectN_GameplayTags.h"
#include "DataAssets/ProjectN_LootDataAsset.h"
#include "Engine/ActorChannel.h"
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

	//UGameplayTagsManager().Get().OnLastChanceToAddNativeTags().AddUObject(this, &UProjectN_InventoryComponent::AddInventoryTags);
}

/*void UProjectN_InventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	//UProjectN_InventoryComponent::EquipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Inventory.EquipItem"), TEXT("Equip item"));
	//UProjectN_InventoryComponent::UnEquipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Inventory.UnEquipItem"), TEXT("Un equip item"));
	//UProjectN_InventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Inventory.DropItem"), TEXT("Drop item"));
	
	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}*/

void UProjectN_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectN_InventoryComponent, InventoryList);
	//DOREPLIFETIME(UProjectN_InventoryComponent, CurrentItemInstance);
	DOREPLIFETIME(UProjectN_InventoryComponent, EquippedItemSlots);
	DOREPLIFETIME(UProjectN_InventoryComponent, Bags);
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
		/*for (const TSubclassOf<UItemStaticClass>& ItemClass : DefaultItems)
        {
        	InventoryList.AddItemByStaticClass(ItemClass);
        }
		for (const TObjectPtr<UProjectN_ItemInstance>& ItemClass : DefaultItemInstance)
        {
        	AddItemByInstance(ItemClass);
        }*/
		if (DefaultLootData)
		{
			for (const auto& Loot : DefaultLootData->LootData)
            {
            	AddItemByStaticClass(Loot.ItemStaticClass, Loot.MaxCount);
            }
		}
		InitBags();
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		//ASC->GenericGameplayEventCallbacks.FindOrAdd(ProjectNGameplayTags::InventoryTag_Equip).AddUObject(this, &UProjectN_InventoryComponent::GameplayEventCallback);
		//ASC->GenericGameplayEventCallbacks.FindOrAdd(ProjectNGameplayTags::InventoryTag_UnEquip).AddUObject(this, &UProjectN_InventoryComponent::GameplayEventCallback);
		//ASC->GenericGameplayEventCallbacks.FindOrAdd(ProjectNGameplayTags::InventoryTag_Drop).AddUObject(this, &UProjectN_InventoryComponent::GameplayEventCallback);
	}
}

/***************************************
 * Main functions to handle inventory
 ***************************************/
/*
void UProjectN_InventoryComponent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	ENetRole NetRole = GetOwnerRole();
	
	if (NetRole == ROLE_Authority)
	{
	//	HandleGameplayEventInternal(*Payload);
	}
	else if (NetRole == ROLE_AutonomousProxy)
	{
	//	ServerHandleGameplayEvent(*Payload);
	}
}
*/

/*
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
}*/

/*void UProjectN_InventoryComponent::EquipTestItem()
{
	if (InventoryList.GetItemsRef().Num() && GetOwner()->HasAuthority())
	{
		//EquipItemByStaticClass(InventoryList.GetItemsRef()[0].ItemInstance->GetItemStaticSubClass());
	}
}*/

void UProjectN_InventoryComponent::AddItemByStaticClass(const TSubclassOf<UItemStaticClass>& ItemStaticDataClass, const int32 ItemStack)
{
	if (GetOwner()->HasAuthority() && ItemStaticDataClass)
	{
		if (ItemStaticDataClass.GetDefaultObject()->CanStack())
		{
			// If item exists and can stack
			if (const FInventoryItem* Item = InventoryList.FindItemByClass(ItemStaticDataClass.GetDefaultObject()))
			{
				Item->ItemInstance->AddItemStack(ItemStack);
				ClientUpdateItemInfo(Item->ItemInstance);
				return;
			}
			
			// If item not exists and can stack
			InventoryList.AddItemByStaticClass(GetOwner(), ItemStaticDataClass, ItemStack);
			return;
		}

		// Else just create item
		for (int32 i = 0; i < ItemStack; i++)
		{
			InventoryList.AddItemByStaticClass(GetOwner(), ItemStaticDataClass, 1);
		}
		
		ClientUpdateItemInfo(InventoryList.FindItemByClass(ItemStaticDataClass.GetDefaultObject())->ItemInstance);
	}
}

/*
void UProjectN_InventoryComponent::AddItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	if (!GetOwner()->HasAuthority() && InItemInstance)
	{
		return;
	}
	
	if (InItemInstance->GetItemStaticClass()->CanStack())
	{
		if (const FInventoryItem* Item = InventoryList.FindItemByClassWithInstance(InItemInstance))
		{
			Item->ItemInstance->AddItemStack(InItemInstance->GetItemStack());
			return;
		}
	}
	
	InventoryList.AddItemByInstance(InItemInstance, InItemInstance->GetItemStack());
}*/

void UProjectN_InventoryComponent::ClientUpdateItemInfo_Implementation(UProjectN_ItemInstance* ItemInstance)
{
	OnUpdateItem.ExecuteIfBound(ItemInstance);
}

void UProjectN_InventoryComponent::ClientRemoveItem_Implementation(UProjectN_ItemInstance* ItemInstance)
{
	OnRemoveItem.ExecuteIfBound(ItemInstance);
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

void UProjectN_InventoryComponent::EquipItemByInstance(UProjectN_ItemInstance* InItemInstance, const EItemSlot InSlot)
{
	AProjectN_CharacterBase* BaseCharacter = Cast<AProjectN_CharacterBase>(Cast<APlayerState>(GetOwner())->GetPawn());
	
	if (GetOwner()->HasAuthority() && IsValid(BaseCharacter))
	{
		// At first check if item can be equipped
		if (!IsEquippableItem(InItemInstance) || !Cast<UProjectN_EquippableItemInstance>(InItemInstance))
		{
			PrintMessage(TEXT("Item is not equippable"));
			return;
		}
		
		// Or if item has allowed slot for equip
		if (!Cast<UEquippableItemStaticClass>(InItemInstance->GetItemStaticClass())->IsAllowedSlot(InSlot))
		{
			PrintMessage(TEXT("Not allowed slot for equip"));
			return;
		}

		// Then check if this item equipped already
		if (IsSlotEquipped(InItemInstance))
		{
			PrintMessage(TEXT("Slot is equipped"));

			FEquippedItemData* FindItemData = FindItemDataByInstance(InItemInstance);

			// Check if this is same slot
			if (FindItemData->ItemSlot == InSlot)
			{
				PrintMessage(TEXT("Same Slot"));
				
				return;
			}

			// Else need un equip old item
			UnEquipItemByInstance(FindItemData->ItemInstance);
		}
		
		if (IsSlotEquipped(InSlot))
		{
			// Check if slot already equipped
			FEquippedItemData* FindItemData = FindItemDataBySlot(InSlot);
	
			PrintMessage(TEXT("Un equip old slot"));
			// Un equip old item
			UnEquipItemByInstance(FindItemData->ItemInstance);
		}

		// Special check for two-handed mode
		if (InSlot == EItemSlot::TwoHand)
		{
			PrintMessage(TEXT("Un equip slot for two-handed mode"));

			const FEquippedItemData* FindMainItemData = FindItemDataBySlot(EItemSlot::MainArm);
			if (FindMainItemData)
			{
				UnEquipItemByInstance(FindMainItemData->ItemInstance);
			}

			const FEquippedItemData* FindAuxiliaryItemData = FindItemDataBySlot(EItemSlot::AuxiliaryArm);
			if (FindAuxiliaryItemData)
			{
				UnEquipItemByInstance(FindAuxiliaryItemData->ItemInstance);
			}
		}

		// Special check if now two-handed mode
		if (InSlot == EItemSlot::MainArm || InSlot == EItemSlot::AuxiliaryArm)
		{
			PrintMessage(TEXT("Un equip slot with two-handed mode"));

			const FEquippedItemData* FindTwoHandItemData = FindItemDataBySlot(EItemSlot::TwoHand);
			if (FindTwoHandItemData)
			{
				UnEquipItemByInstance(FindTwoHandItemData->ItemInstance);
			}
		}
		
		// Just check if item has in our inventory
		const FInventoryItem* Item = InventoryList.FindItemByInstance(InItemInstance);
		if (Item)
		{
			Cast<UProjectN_EquippableItemInstance>(InItemInstance)->OnEquip(BaseCharacter, Cast<UEquippableItemStaticClass>(InItemInstance->GetItemStaticClass())->GetSocketsToAttach(InSlot)/*, Tag*/);

			// Add item data to list of equipped items
			AddItemToSlot(InSlot, InItemInstance);
			ApplyItemStats(InItemInstance);
			
			if (const UWeaponItemStaticClass* WeaponItemStaticClass = Cast<UWeaponItemStaticClass>(InItemInstance->GetItemStaticClass()))
			{
				// Update weapon abilities in accordance with equip mode
				UpdateWeaponMode();
				
				const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());
				
				//const FGameplayTag GeneratedTag = FGameplayTag::RequestGameplayTag(FName(*AssociatedSlotWithWeaponSlotTag.Find(FindItemDataByInstance(InItemInstance)->ItemSlot)->ToString() + WeaponItemStaticClass->GetWeaponTypeTag().ToString()));
				//ASCInterface->GetAbilitySystemComponent()->AddLooseGameplayTag(GeneratedTag);
				
				// Add weapon tag
				//ASCInterface->GetAbilitySystemComponent()->AddLooseGameplayTag(WeaponItemStaticClass->GetWeaponTypeTag());
				
				// Add weapon slot tag
				//ASCInterface->GetAbilitySystemComponent()->AddLooseGameplayTag(*AssociatedSlotWithWeaponSlotTag.Find(FindItemDataByInstance(InItemInstance)->ItemSlot));
			}
			
			PrintMessage(TEXT("Slot is equipped and added"));
		}

		
		/*for (const FInventoryItem& Item : InventoryList.GetItemsRef())
		{
			if (Item.ItemInstance == InItemInstance)
			{
				// Find associated tag for slot
				FGameplayTag Tag = FGameplayTag();
				for (const auto& Pair : AssociatedTagWithSlot)
				{
					if (Pair.Value == InSlot)
					{
						Tag = Pair.Key;
						break;
					}
				}
				Item.ItemInstance->OnEquip(BaseCharacter, *Cast<UEquippableItemStaticClass>(Item.ItemInstance->GetItemStaticClass())->GetSocketsToAttach().Find(InSlot), Tag);

				// Add item data to list of equipped items
				AddItemToSlot(InSlot, Item.ItemInstance);

				PrintMessage(TEXT("Slot is equipped and added"));
				break;
			}
		}*/
	}
}

void UProjectN_InventoryComponent::UnEquipItemByInstance(UProjectN_ItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		if (!IsSlotEquipped(InItemInstance) || !Cast<UProjectN_EquippableItemInstance>(InItemInstance))
		{
			return;
		}
		
		Cast<UProjectN_EquippableItemInstance>(InItemInstance)->OnUnEquip();
		
		RemoveItemStats(InItemInstance);
		
		if (const UWeaponItemStaticClass* WeaponItemStaticClass = Cast<UWeaponItemStaticClass>(InItemInstance->GetItemStaticClass()))
		{				
			const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());
			
			// Remove weapon tag
			//ASCInterface->GetAbilitySystemComponent()->RemoveLooseGameplayTag(WeaponItemStaticClass->GetWeaponTypeTag());

			// Remove weapon slot tag
			//ASCInterface->GetAbilitySystemComponent()->RemoveLooseGameplayTag(*AssociatedSlotWithWeaponSlotTag.Find(FindItemDataByInstance(InItemInstance)->ItemSlot));
		}

		// Remove item data from list of equipped items
		RemoveSlot(FindItemDataByInstance(InItemInstance)->ItemSlot);

		// Update weapon abilities in accordance with equip mode
		UpdateWeaponMode();
		
		PrintMessage(TEXT("Slot is un equipped and removed"));
	}
}

void UProjectN_InventoryComponent::DropItem(UProjectN_ItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		//if (IsValid(CurrentItemInstance) && Cast<UProjectN_EquippableItemInstance>(InItemInstance))
		{
			Cast<UProjectN_EquippableItemInstance>(InItemInstance)->OnDrop();
			ClientRemoveItem(InItemInstance);
			//CurrentItemInstance = nullptr;
		}
	}
}

void UProjectN_InventoryComponent::PrintMessage(const FString& InText)
{
	FVector2D MessageSize = FVector2D(1.f, 1.f);
	//GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Blue, InText, true, MessageSize);
	UKismetSystemLibrary::PrintString(this, InText, true, true, FColor::Blue);
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
	const FEquippedItemData* FindItem = FindItemDataByInstance(InItemInstance);
	return FindItem != nullptr;
}

FEquippedItemData* UProjectN_InventoryComponent::FindItemDataBySlot(const EItemSlot InItemSlot)
{
	/*return EquippedItemSlots.EquippedItems.FindByPredicate([InItemSlot](const FEquippedItemData& EquippedItemData)
	{
		return EquippedItemData.ItemSlot == InItemSlot;
	});*/

	for (FEquippedItemData& Item : EquippedItemSlots.EquippedItems)
	{
		if (Item.ItemSlot == InItemSlot)
		{
			return &Item;
		}
	}
	return nullptr;
}

FEquippedItemData* UProjectN_InventoryComponent::FindItemDataByInstance(const UProjectN_ItemInstance* InItemInstance)
{
	/*return EquippedItemSlots.EquippedItems.FindByPredicate([InItemInstance](const FEquippedItemData& EquippedItemData)
	{
		return EquippedItemData.ItemInstance == InItemInstance;
	});*/

	for (FEquippedItemData& Item : EquippedItemSlots.EquippedItems)
	{
		if (Item.ItemInstance == InItemInstance)
		{
			return &Item;
		}
	}
	return nullptr;
}

FVector UProjectN_InventoryComponent::FindSocketLocationBySlot(const EItemSlot ItemSlot)
{
	if (ItemSlot == EItemSlot::None)
	{
		return FVector::ZeroVector;
	}
	
	FEquippedItemData* FindItem = FindItemDataBySlot(ItemSlot);
	if (FindItem != nullptr)
	{
		// Get the item actor and find the sockets
		return Cast<UProjectN_EquippableItemInstance>(FindItem->ItemInstance)->GetItemSocketLocationForProjectile();
	}
	
	return FVector::ZeroVector;
}

AProjectN_WeaponActor* UProjectN_InventoryComponent::GetEquippedWeaponActorBySlot(const EItemSlot InItemSlot)
{
	if (const FEquippedItemData* ItemData = FindItemDataBySlot(InItemSlot))
	{
		return Cast<AProjectN_WeaponActor>(Cast<UProjectN_EquippableItemInstance>(ItemData->ItemInstance)->GetItemActor());
	}
	return nullptr;
}

FGameplayTag UProjectN_InventoryComponent::GetWeaponTypeBySlot(const EItemSlot InItemSlot)
{
	if (const FEquippedItemData* ItemData = FindItemDataBySlot(InItemSlot))
	{
		return Cast<UWeaponItemStaticClass>(ItemData->ItemInstance->GetItemStaticClass())->GetWeaponTypeTag();
	}
	return FGameplayTag();
}

void UProjectN_InventoryComponent::RemoveSlot(const EItemSlot InItemSlot)
{
	for (auto ItemIter = EquippedItemSlots.EquippedItems.CreateIterator(); ItemIter; ++ItemIter)
	{
		FEquippedItemData& Item = *ItemIter;
		if (Item.ItemSlot == InItemSlot)
		{
			ItemIter.RemoveCurrent();
			EquippedItemSlots.MarkArrayDirty();
			break;
		}
	}
}

void UProjectN_InventoryComponent::AddItemToSlot(const EItemSlot InItemSlot, UProjectN_ItemInstance* InItemInstance)
{
	FEquippedItemData& NewItem = EquippedItemSlots.EquippedItems.AddDefaulted_GetRef();
	NewItem.ItemInstance = InItemInstance;
	NewItem.ItemSlot = InItemSlot;

	EquippedItemSlots.MarkItemDirty(NewItem);
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

/*********************************
 *  Weapon abilities managing
 *********************************/
void UProjectN_InventoryComponent::UpdateWeaponMode()
{
	const FEquippedItemData* MainArmItemData = FindItemDataBySlot(EItemSlot::MainArm);
	const FEquippedItemData* AuxiliaryArmItemData = FindItemDataBySlot(EItemSlot::AuxiliaryArm);
	const FEquippedItemData* TwoHandItemData = FindItemDataBySlot(EItemSlot::TwoHand);

	UItemStaticClass* MainArmItemStaticClass = MainArmItemData? MainArmItemData->ItemInstance->GetItemStaticClass() : nullptr;
	UItemStaticClass* AuxiliaryArmItemStaticClass = AuxiliaryArmItemData? AuxiliaryArmItemData->ItemInstance->GetItemStaticClass() : nullptr;
	UItemStaticClass* TwoHandItemStaticClass = TwoHandItemData? TwoHandItemData->ItemInstance->GetItemStaticClass() : nullptr;

	RemoveWeaponAbilities(EWeaponMode::Single);
	RemoveWeaponAbilities(EWeaponMode::Dual);
	RemoveWeaponAbilities(EWeaponMode::TwoHand);
	
	if (TwoHandItemStaticClass)
	{
		GiveWeaponAbilities(TwoHandItemStaticClass, EWeaponMode::TwoHand, true, true);

		return;
	}

	if (MainArmItemStaticClass && AuxiliaryArmItemStaticClass && Cast<UWeaponItemStaticClass>(MainArmItemStaticClass)->GetWeaponTypeTag() == Cast<UWeaponItemStaticClass>(AuxiliaryArmItemStaticClass)->GetWeaponTypeTag())
	{
		GiveWeaponAbilities(MainArmItemStaticClass, EWeaponMode::Dual, true, true);

		return;
	}

	if (MainArmItemStaticClass)
	{
		GiveWeaponAbilities(MainArmItemStaticClass, EWeaponMode::Single, true, false);
	}
	
	if (AuxiliaryArmItemStaticClass)
	{
		GiveWeaponAbilities(AuxiliaryArmItemStaticClass, EWeaponMode::Single, false, true);
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

void UProjectN_InventoryComponent::GiveWeaponAbilities(UItemStaticClass* WeaponItemStaticClass, const EWeaponMode WeaponMode, const bool bAddForMainHand, const bool bAddForAuxiliaryHand)
{
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}
	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	
	FWeaponAbilitiesInfo WeaponAbilitiesInfoOut;
	if (Cast<UWeaponItemStaticClass>(WeaponItemStaticClass)->GetWeaponAbilitiesInfo(WeaponMode, WeaponAbilitiesInfoOut))
	{
		FGrantedAbilityHandles AbilityHandles;

		if (bAddForMainHand)
		{
			AbilityHandles.AbilitySpecs.Add(ASC->AddAbility(WeaponAbilitiesInfoOut.MainWeaponAbility, ProjectNGameplayTags::Input_LMB));
		}
		if (bAddForAuxiliaryHand)
		{
			AbilityHandles.AbilitySpecs.Add(ASC->AddAbility(WeaponAbilitiesInfoOut.AuxiliaryWeaponAbility, ProjectNGameplayTags::Input_RMB));
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

void UProjectN_InventoryComponent::ApplyItemStats(const UProjectN_ItemInstance* InItem) const
{	
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}
	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	
	// Apply item attributes
	for (const TTuple<FGameplayTag, float> Attribute : Cast<UEquippableItemStaticClass>(InItem->GetItemStaticClass())->GetItemBonusAttributes())
	{
		ASC->ServerAddToAttributeByTag(Attribute.Key, Attribute.Value);
	}
}

void UProjectN_InventoryComponent::RemoveItemStats(const UProjectN_ItemInstance* InItem) const
{	
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!ASCInterface)
	{
		return;
	}
	
	UProjectN_AbilitySystemComponent* ASC = Cast<UProjectN_AbilitySystemComponent>(ASCInterface->GetAbilitySystemComponent());
	
	// Apply item attributes
	for (const TTuple<FGameplayTag, float> Attribute : Cast<UEquippableItemStaticClass>(InItem->GetItemStaticClass())->GetItemBonusAttributes())
	{
		ASC->ServerAddToAttributeByTag(Attribute.Key, -Attribute.Value);
	}
}

/*
AProjectN_ItemActor_Base* UProjectN_InventoryComponent::GetWeaponActorByType(const EWeaponType InType)
{
	for (const auto& ItemData : EquippedItemSlots.EquippedItems)
	{
		if (const UWeaponItemStaticClass* WeaponItemStaticClass = Cast<UWeaponItemStaticClass>(ItemData.ItemInstance->GetItemStaticClass()))
		{
			if (WeaponItemStaticClass->GetWeaponType() == InType)
			{
				return Cast<UProjectN_EquippableItemInstance>(ItemData.ItemInstance)->GetItemActor();
			}
		}
	}
	
	return nullptr;
}
*/
TMap<FGameplayTag, float> UProjectN_InventoryComponent::GetWeaponDamageTypesForSlot(const EItemSlot InItemSlot)
{
	if (const FEquippedItemData* ItemData = FindItemDataBySlot(InItemSlot))
	{
		return Cast<UWeaponItemStaticClass>(ItemData->ItemInstance->GetItemStaticClass())->GetWeaponDamageTypes();
	}
	
	TMap<FGameplayTag, float> EmptyMap;
	return EmptyMap;
}

/*
float UProjectN_InventoryComponent::GetWeaponMaxDamageForSlot(const EItemSlot InItemSlot)
{
	if (const FEquippedItemData* ItemData = FindItemDataBySlot(InItemSlot))
	{
		return *Cast<UWeaponItemStaticClass>(ItemData->ItemInstance->GetItemStaticClass())->GetItemBonusAttributes();
	}
	return 0.f;
}
*/

//////////////////////////////////////////////////////
// Wow realisation
void UProjectN_InventoryComponent::InitBags()
{
	for (int32 i = 0; i < BagsDefaultID.Num(); i++)
	{
		AddBag(BagsDefaultID[i]);
		/*static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindBagFromDataTable"));
		
		if (const FBagDefinition* DageDef = BagDataTable.LoadSynchronous()->FindRow<FBagDefinition>(BagsDefaultID[i], Context, false))
		{
			FBagData NewBag;
			NewBag.BagItemID = BagsDefaultID[i];
			NewBag.Slots.SetNum(DageDef->NumSlots);
			Bags.Add(MoveTemp(NewBag));
			
			if (OnBagAdded.IsBound())
			{
				OnBagAdded.Execute(i, DageDef->NumSlots);
			}
		}*/
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
		FBagData NewBag;
		NewBag.BagID = Bags.Num();
		NewBag.BagItemID = InBagItemID;
		NewBag.Slots.SetNum(BagDef->NumSlots);
		BroadcastBagChange(NewBag.BagID, BagDef->NumSlots);
		
		Bags.Add(MoveTemp(NewBag));

		
		/*if (OnBagAdded.IsBound())
		{
			OnBagAdded.Execute(Bags.Num() - 1, BagDef->NumSlots);
		}*/
	}
}

void UProjectN_InventoryComponent::RemoveBag(const int32 BagID)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	const FBagData* BagToRemove= nullptr;
	for (int32 i = 0; i < Bags.Num(); i++)
	{
		if (Bags[i].BagID == BagID)
		{
			Bags.RemoveAt(i);
			BroadcastBagChange(i, 0);
			return;
		}
	}
	
		
		/*if (OnBagRemoved.IsBound())
		{
			OnBagRemoved.Execute(Bags.Num(), 0);
		}*/
}

bool UProjectN_InventoryComponent::TryAddItemToFirstFreeSlot(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	for (FBagData& Bag : Bags)
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

				BroadcastSlotChange(Bag.BagID, i, Bag.Slots[i]);
				
				return true;
			}
		}
	}
	return false;
}

bool UProjectN_InventoryComponent::TryAddItemToStack(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindBagFromDataTable"));
		
	if (const FItemDefinition* ItemDef = BagDataTable.LoadSynchronous()->FindRow<FItemDefinition>(ItemID, Context, false))
	{
		const int32 ItemMaxStack = ItemDef->MaxStack;

		for (FBagData& Bag : Bags)
		{
			for (int32 i = 0; i < Bag.Slots.Num(); i++)
			{
				if (Bag.Slots[i].ItemID == ItemID && Bag.Slots[i].EntryType == ItemType)
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
		
		/*if (OnItemAdded.IsBound())
		{
			OnItemAdded.Execute(false);
		}*/
	}
	return false;
}

bool UProjectN_InventoryComponent::TryAddItem(const FName& ItemID, const EEntryType ItemType, const int32 Quantity)
{
	if (ItemType == EEntryType::Item)
	{
		return TryAddItemToStack(ItemID, ItemType, Quantity);
	}
	return TryAddItemToFirstFreeSlot(ItemID, ItemType, Quantity);
}

void UProjectN_InventoryComponent::ReplaceItemInBag(const int32 FromBagID, const int32 ToBagID, const int32 FromSlotIndex, const int32 ToSlotIndex)
{
	FInventorySlotData FromSlotData;
	FInventorySlotData ToSlotData;

	for (FBagData& Bag : Bags)
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

	for (FBagData& Bag : Bags)
	{
		if (Bag.BagID == FromBagID)
		{
			Bag.Slots[FromSlotIndex] = MoveTemp(ToSlotData);

			BroadcastSlotChange(Bag.BagID, ToSlotIndex, Bag.Slots[ToSlotIndex]);
		}
		
		if (Bag.BagID == ToBagID)
		{
			Bag.Slots[ToSlotIndex] = MoveTemp(FromSlotData);

			BroadcastSlotChange(Bag.BagID, ToSlotIndex, Bag.Slots[ToSlotIndex]);
		}
	}
}

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
