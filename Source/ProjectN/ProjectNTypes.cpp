#include "ProjectN/ProjectNTypes.h"

#include "Inventory/ProjectN_InventoryComponent.h"
#include "UI/Widgets/Composite/ProjectN_CompositeBase.h"

void FBagList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	for (const int32 Index : RemovedIndices)
	{
		InventoryComponent->OnInventorySlotRemoved.ExecuteIfBound(Slots[Index].BagIndex, Slots[Index].SlotIndex, Slots[Index]);
	}
}

void FBagList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	for (const int32 Index : AddedIndices)
	{
		InventoryComponent->OnInventorySlotChange.ExecuteIfBound(Slots[Index].BagIndex, Slots[Index].SlotIndex, Slots[Index]);
	}
}

void FBagList::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	for (const int32 Index : ChangedIndices)
	{
		if (Slots[Index].ItemID.IsNone())
		{
			InventoryComponent->OnInventorySlotRemoved.ExecuteIfBound(Slots[Index].BagIndex, Slots[Index].SlotIndex, Slots[Index]);
		}
		else
		{
			InventoryComponent->OnInventorySlotChange.ExecuteIfBound(Slots[Index].BagIndex, Slots[Index].SlotIndex, Slots[Index]);
		}
	}
}

void UItemManifest::AssimilateInventoryFragments(UProjectN_CompositeBase* Composite) const
{
	const auto& InventoryFragments = GetAllFragmentsOfType<FInventoryFragment>();
	for (const auto& Fragment : InventoryFragments)
	{
		Composite->ApplyFunction([Fragment](UProjectN_CompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}

void FInventoryFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite))
	{
		return;
	}
	
	Composite->Expand();
}

bool FInventoryFragment::MatchesWidgetTag(const UProjectN_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}
