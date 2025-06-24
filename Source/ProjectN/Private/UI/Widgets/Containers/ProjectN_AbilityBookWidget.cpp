// N Chedurov All Rights Reserved

#include "UI/Widgets/Containers/ProjectN_AbilityBookWidget.h"

#include "Components/GridPanel.h"
#include "UI/Widgets/Slots/ProjectN_EntrySlotBase.h"
#include "UI/Widgets/Slots/AbilityBookSlot/ProjectN_AbilityBookSlot.h"

UProjectN_AbilityBookSlot* UProjectN_AbilityBookWidget::FindAbility(const FName& AbilityID)
{
	for (const TObjectPtr<UProjectN_AbilityBookSlot>& Ability : AbilitySlots)
	{
		if (Ability->GetItemID() == AbilityID)
		{
			return Ability;
		}
	}

	return nullptr;
}

UProjectN_AbilityBookSlot* UProjectN_AbilityBookWidget::AddNewAbility()
{
	UProjectN_AbilityBookSlot* NewAbility = CreateWidget<UProjectN_AbilityBookSlot>(GetOwningPlayer(), AbilitySlotClass);
	
	AbilitySlots.Add(NewAbility);
	AbilityBookGrid->AddChildToGrid(NewAbility, (AbilitySlots.Num() - 1) / Columns, (AbilitySlots.Num() - 1) % Columns);
	
	NewAbility->OnSlotHovered.AddDynamic(this, &UProjectN_AbilityBookWidget::OnSlotHovered);
	NewAbility->OnSlotUnhovered.AddDynamic(this, &UProjectN_AbilityBookWidget::OnSlotUnhovered);
	return NewAbility;
}