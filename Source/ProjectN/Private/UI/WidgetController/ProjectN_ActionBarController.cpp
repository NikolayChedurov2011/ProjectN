// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_ActionBarController.h"

#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"

void UProjectN_ActionBarController::BindCallbacksToResponce()
{

	/*if (InventoryComponent)
	{
		

		InventoryComponent->OnBagChanged.BindLambda([this, InventoryComponent](const int32 BagID, const int32 BagSlots)
		{
			if (OnUpdateBag.IsBound())
			{
				OnUpdateBag.Broadcast(BagID, BagSlots);
			}
		});

		InventoryComponent->OnSlotChange.BindLambda([this, InventoryComponent](const int32 BagID, const int32 BagSlots, const FInventorySlotData& ItemData)
		{
			if (OnUpdateSlot.IsBound())
			{
				OnUpdateSlot.Broadcast(BagID, BagSlots, ItemData);
			}
		});
	}*/
}

void UProjectN_ActionBarController::BroadcastInitialValues()
{
	FProjectNActionSlotInfo NewActionSlotInfo;
	NewActionSlotInfo.EntryType = EEntryType::Ability;
	NewActionSlotInfo.InputAction = ProjectNGameplayTags::Input_Slot0;
	NewActionSlotInfo.ItemID = FName("ability_001");
	NewActionSlotInfo.ActionSlotIndex = 9;

	ActionSlots.Add(NewActionSlotInfo);

	FProjectNActionSlotInfo NewActionSlotInfo2;
	NewActionSlotInfo2.EntryType = EEntryType::Ability;
	NewActionSlotInfo2.InputAction = ProjectNGameplayTags::Input_Slot5;
	NewActionSlotInfo2.ItemID = FName("ability_002");
	NewActionSlotInfo2.ActionSlotIndex = 4;

	ActionSlots.Add(NewActionSlotInfo2);
	
	for (const FProjectNActionSlotInfo& ActionSlot : ActionSlots)
	{
		AddAbility(ActionSlot);
		if (OnUpdateActionSlot.IsBound())
		{
			OnUpdateActionSlot.Broadcast(ActionSlot);
		}
	}
	/*if (InventoryComponent)
	{
		

		// TODO: Init inventory items
		for (int32 i = 0; i < InventoryComponent->GetBags().Num(); i++)
		{
			if (OnUpdateBag.IsBound())
			{
				OnUpdateBag.Broadcast(i, InventoryComponent->GetBags()[i].Slots.Num());
			}
		}
	}*/
}

void UProjectN_ActionBarController::AddAbility(const FProjectNActionSlotInfo& ActionSlotInfo) const
{
	if (UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent))
	{
		switch (ActionSlotInfo.EntryType)
		{
		case EEntryType::Ability :
			if (const FAbilityDefinition* ItemDef = GetAbilityData(ActionSlotInfo.ItemID))
			{
				ProjectN_AbilitySystemComponent->ServerAddAbility(ItemDef->Ability, ActionSlotInfo.InputAction);
				return;
			}
		case EEntryType::Item :
			if (const FItemDefinition* ItemDef = GetItemData(ActionSlotInfo.ItemID))
			{
				ProjectN_AbilitySystemComponent->ServerAddAbility(ItemDef->UseItemAbility, ActionSlotInfo.InputAction);
				return;
			}
		case EEntryType::Equipment :
			//if (const FEquippableItemDefinition* ItemDef = GetEquippableItemData(ActionSlotInfo.ItemID))
			{
				return;
			}
		case EEntryType::Weapon :
			//if (const FWeaponItemDefinition* ItemDef = GetWeaponData(ActionSlotInfo.ItemID))
			{
				return;
			}
		case EEntryType::Bag :
			//if (const FBagDefinition* ItemDef = GetBagData(ActionSlotInfo.ItemID))
			{
				return;
			}
		default: ;
		}
	}	
}

const FAbilityDefinition* UProjectN_ActionBarController::GetAbilityData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindAbilityDefFromDataTable"));
	
	if (const FAbilityDefinition* AbilityDef = AbilityDataTable.LoadSynchronous()->FindRow<FAbilityDefinition>(ItemID, Context, false))
	{
		return AbilityDef;
	}
	return nullptr;
}

const FItemDefinition* UProjectN_ActionBarController::GetItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindItemFromDataTable"));
	
	if (const FItemDefinition* ItemDef = ItemsDataTable.LoadSynchronous()->FindRow<FItemDefinition>(ItemID, Context, false))
	{
		return ItemDef;
	}
	return nullptr;
}

const FEquippableItemDefinition* UProjectN_ActionBarController::GetEquippableItemData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindEquippableItemFromDataTable"));
	
	if (const FEquippableItemDefinition* WeaponItemDef = EquippableItemDataTable.LoadSynchronous()->FindRow<FEquippableItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FWeaponItemDefinition* UProjectN_ActionBarController::GetWeaponData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindWeaponFromDataTable"));
	
	if (const FWeaponItemDefinition* WeaponItemDef = WeaponItemsDataTable.LoadSynchronous()->FindRow<FWeaponItemDefinition>(ItemID, Context, false))
	{
		return WeaponItemDef;
	}
	return nullptr;
}

const FBagDefinition* UProjectN_ActionBarController::GetBagData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindBagFromDataTable"));
	
	if (const FBagDefinition* BagDef = BagDataTable.LoadSynchronous()->FindRow<FBagDefinition>(ItemID, Context, false))
	{
		return BagDef;
	}
	return nullptr;
}