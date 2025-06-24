// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_AbilityBookController.h"

#include "ProjectN_GameplayTags.h"
#include "UI/Widgets/Containers/ProjectN_AbilityBookWidget.h"
#include "UI/Widgets/Slots/AbilityBookSlot/ProjectN_AbilityBookSlot.h"

void UProjectN_AbilityBookController::BindCallbacksToResponce()
{
	// TODO: bind to get new ability
}

void UProjectN_AbilityBookController::BroadcastInitialValues()
{
	// TODO: Load saved abilities from save
	AddAbility(FName("id_ability_eye_fire"));
}

void UProjectN_AbilityBookController::SetAbilityBookWidgetRef(UProjectN_AbilityBookWidget* NewAbilityBookWidget)
{
	AbilityBookWidget = NewAbilityBookWidget;
}

void UProjectN_AbilityBookController::AddAbility(const FName& AbilityID) const
{
	const FEntriesDefinition* AbilityEntriesDefinition = GetEntryManifest(AbilityID);
			
	if (!AbilityEntriesDefinition)
	{
		return;
	}
			
	const FTypeFragment* TypeFragment = GetFragment<FTypeFragment>(*AbilityEntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Type);

	if (TypeFragment->GetEntryType() != EEntryType::Ability)
	{
		return;
	}
	
	UProjectN_AbilityBookSlot* NewAbility = AbilityBookWidget->AddNewAbility();
	const FIconFragment* IconFragment = GetFragment<FIconFragment>(*AbilityEntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Icon);
			
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(IconFragment->GetIcon());
	NewAbility->SetAbilityIcon(NewBrush);
	NewAbility->SetItemID(AbilityID);
}

/*******************
*   Getters
********************/
FEntriesDefinition* UProjectN_AbilityBookController::GetEntryManifest(const FName& ItemID) const
{
	if (!Entries) return nullptr;
	
	const FString Context = FString(TEXT("UProjectN_AbilityBookController::FindEntryFromDataTable"));
	
	if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(ItemID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}