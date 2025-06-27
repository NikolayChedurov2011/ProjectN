#include "ProjectN/ProjectNTypes.h"

#include "Inventory/ProjectN_InventoryComponent.h"
#include "UI/Widgets/Composite/ProjectN_Leaf_Labeled.h"
#include "UI/Widgets/Composite/ProjectN_Leaf_Text.h"

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
		if (!Fragment)
		{
			return;
		}
		
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

void FTextFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	FInventoryFragment::Assimilate(Composite);

	if (!MatchesWidgetTag(Composite))
	{
		return;
	}

	if (UProjectN_Leaf_Text* TextLeaf = Cast<UProjectN_Leaf_Text>(Composite))
	{
		TextLeaf->SetText(TextFragment);
	}
}

void FLabeledFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	FInventoryFragment::Assimilate(Composite);

	if (!MatchesWidgetTag(Composite))
	{
		return;
	}

	if (UProjectN_Leaf_Labeled* LabeledLeaf = Cast<UProjectN_Leaf_Labeled>(Composite))
	{
		LabeledLeaf->SetLabelText(LabelText, false);
		LabeledLeaf->SetValueText(FText::AsNumber(Value), false);
	}
}

void FModifierFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	FInventoryFragment::Assimilate(Composite);

	for (const TInstancedStruct<FLabeledFragment>& Modifier : Modifiers)
	{
		const FLabeledFragment& ModifierRef = Modifier.Get();
		ModifierRef.Assimilate(Composite);
	}
}

void FAbilityFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	FInventoryFragment::Assimilate(Composite);

	if (Cooldown.IsValid())
	{
		Cooldown.Get<FLabeledFragment>().Assimilate(Composite);
	}
	
	if (Cost.IsValid())
	{
		Cost.Get<FLabeledFragment>().Assimilate(Composite);
	}

	if (Value.IsValid())
	{
		Value.Get<FLabeledFragment>().Assimilate(Composite);
	}
}

void FEquippingFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	FInventoryFragment::Assimilate(Composite);

	if (BonusModifiers.IsValid())
	{
		BonusModifiers.Get<FModifierFragment>().Assimilate(Composite);
	}
}

void FWeaponFragment::Assimilate(UProjectN_CompositeBase* Composite) const
{
	FInventoryFragment::Assimilate(Composite);

	if (WeaponDamageModifiers.IsValid())
	{
		WeaponDamageModifiers.Get<FLabeledFragment>().Assimilate(Composite);
	}
}


