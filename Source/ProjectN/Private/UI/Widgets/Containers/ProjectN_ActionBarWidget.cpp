// N Chedurov All Rights Reserved


#include "UI/Widgets/Containers/ProjectN_ActionBartWidget.h"
#include "UI/Widgets/Slots/ActionSlot/ProjectN_ActionSlot.h"

void UProjectN_ActionBartWidget::BindToActionSlotsEvent()
{
	for (const TObjectPtr<UProjectN_ActionSlot> ActionSlot : ActionSlots)
	{
		ActionSlot->OnSlotHovered.AddDynamic(this, &UProjectN_ActionBartWidget::OnSlotHovered);
		ActionSlot->OnSlotUnhovered.AddDynamic(this, &UProjectN_ActionBartWidget::OnSlotUnhovered);
	}
}

void UProjectN_ActionBartWidget::ClearActionSlot(const int32 SlotIndex)
{
	ActionSlot(SlotIndex)->ClearSlot();
}

void UProjectN_ActionBartWidget::ClearActionSlot(const FGameplayTag& InputTag)
{
	ActionSlot(InputTag)->ClearSlot();
}

UProjectN_ActionSlot* UProjectN_ActionBartWidget::ActionSlot(const int32 SlotIndex)
{
	for (const TObjectPtr<UProjectN_ActionSlot>& ActionSlot : ActionSlots)
	{
		if (ActionSlot->GetSlotIndex() == SlotIndex)
		{
			return ActionSlot;
		}
	}
	return nullptr;
}

UProjectN_ActionSlot* UProjectN_ActionBartWidget::ActionSlot(const FGameplayTag& InputTag)
{
	for (const TObjectPtr<UProjectN_ActionSlot>& ActionSlot : ActionSlots)
	{
		if (ActionSlot->GetInputTag() == InputTag)
		{
			return ActionSlot;
		}
	}
	return nullptr;
}