// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AbilityBookController.h"

void UProjectN_AbilityBookController::BindCallbacksToResponce()
{
	// TODO: bind to get new ability
}

void UProjectN_AbilityBookController::BroadcastInitialValues()
{
	// TODO: Load saved abilities from save
	AddAbility(FName("ability_001"));
}

void UProjectN_AbilityBookController::AddAbility(const FName& AbilityID)
{
	FAbilitySlotData NewActionSlotInfo;
	NewActionSlotInfo.ItemType = EEntryType::Ability;
	NewActionSlotInfo.ItemID = AbilityID;
	
	if (NewActionSlotInfo.ItemIcon == nullptr)
	{
		LoadAbilityIcon(NewActionSlotInfo);
	}
	
	Abilities.Add(MoveTemp(NewActionSlotInfo));
	
	if (OnUpdateAbilitySlot.IsBound())
	{
		OnUpdateAbilitySlot.Broadcast(NewActionSlotInfo);
	}
}

void UProjectN_AbilityBookController::LoadAbilityIcon(FAbilitySlotData& ActionSlotInfo) const
{
	if (const FAbilityDefinition* ItemDef = GetAbilityData(ActionSlotInfo.ItemID))
	{
		ActionSlotInfo.ItemIcon = ItemDef->ItemIcon;
	}
}

/*******************
*   Getters
********************/
const FAbilityDefinition* UProjectN_AbilityBookController::GetAbilityData(const FName& ItemID) const
{
	static const FString Context = FString(TEXT("UProjectN_InventoryComponent::FindAbilityDefFromDataTable"));
	
	if (const FAbilityDefinition* AbilityDef = AbilityDataTable.LoadSynchronous()->FindRow<FAbilityDefinition>(ItemID, Context, false))
	{
		return AbilityDef;
	}
	return nullptr;
}