// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_InventoryController.h"

#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/ProjectN_InventoryComponent.h"
#include "UI/Widgets/Containers/ProjectN_EquipmentWidget.h"
#include "UI/Widgets/Containers/ProjectN_InventoryWidget.h"
#include "UI/Widgets/Slots/EquipSlot/ProjectN_EquipSlot.h"
#include "UI/Widgets/Slots/InventorySlot/ProjectN_InventorySlot.h"

void UProjectN_InventoryController::BindCallbacksToResponce()
{
	InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(AbilitySystemComponent);

	if (InventoryComponent)
	{
		InventoryComponent->OnBagChanged.BindLambda([this](const FGuid BagIndex, const int32 SlotsNum)
		{
			InventoryWidget->InitNewBag(BagIndex, SlotsNum);
		});

		InventoryComponent->OnEquipSlotChange.BindLambda([this](const FEquipSlotData& ItemData)
		{
			if (UProjectN_EquipSlot* EquipmentSlot = EquipmentWidget->FindEquipmentSlot(ItemData.EquipSlot))
			{
				const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemData.ItemID);
				if (!EntriesDefinition)
				{
					return;
				}
				
				const FIconFragment* IconFragment = GetFragment<FIconFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Icon);
				
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(IconFragment->GetIcon());
				EquipmentSlot->SetItemIcon(NewBrush);
				EquipmentSlot->SetItemID(ItemData.ItemID);
			}
		});

		InventoryComponent->OnEquipSlotRemoved.BindLambda([this](const FEquipSlotData& ItemData)
		{
			EquipmentWidget->ClearEquipmentSlot(ItemData.EquipSlot);
		});

		InventoryComponent->OnInventorySlotChange.BindLambda([this, ProjectN_AbilitySystemComponent](const FGuid BagIndex, const int32 SlotIndex, const FInventorySlotData& ItemData)
		{
			if (UProjectN_InventorySlot* InventorySlot = InventoryWidget->FindInventorySlot(BagIndex, SlotIndex))
			{
				const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemData.ItemID);
				if (!EntriesDefinition)
				{
					return;
				}
				
				const FIconFragment* IconFragment = GetFragment<FIconFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Icon);
				const FAbilityFragment* AbilityFragment = GetFragment<FAbilityFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Ability);
				
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(IconFragment->GetIcon());
				InventorySlot->SetItemIcon(NewBrush);
				InventorySlot->SetStackCount(ItemData.Quantity);
				InventorySlot->SetItemID(ItemData.ItemID);

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, AbilityFragment, InventorySlot, ProjectN_AbilitySystemComponent]()
				{
					if (AbilityFragment)
					{
						InventorySlot->SetCooldownTag(AbilityFragment->GetCooldownTag());
						InventorySlot->SetCooldownBaseValue(AbilityFragment->GetCooldownValue());
						InventorySlot->SetCooldownValueRemaining(ProjectN_AbilitySystemComponent->FindCooldownRemaining(AbilityFragment->GetCooldownTag()));
					}
				}), 0.3f,false);
			}
		});
		
		InventoryComponent->OnInventorySlotRemoved.BindLambda([this](const FGuid BagIndex, const int32 SlotIndex, const FInventorySlotData& ItemData)
		{
			InventoryWidget->ClearInventorySlot(BagIndex, SlotIndex);
		});
	}
}

void UProjectN_InventoryController::BroadcastInitialValues()
{
	InventoryComponent = PlayerState->FindComponentByClass<UProjectN_InventoryComponent>();

	if (InventoryComponent)
	{
		InventoryComponent->ServerInitBags();
	}
}

void UProjectN_InventoryController::SetInventoryWidgetRef(UProjectN_InventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UProjectN_InventoryController::SetEquipmentWidgetRef(UProjectN_EquipmentWidget* NewEquipmentWidget)
{
	EquipmentWidget = NewEquipmentWidget;
}

/*********************************
 *  Items manage
 *********************************/
void UProjectN_InventoryController::TryAddItem(const FName ItemID, const int32 Quantity) const
{
	InventoryComponent->ServerTryAddItem(ItemID, Quantity);
}

void UProjectN_InventoryController::TryAddItemToSlot(const FGuid BagIndex, const int32 SlotIndex, const FName& ItemID, const int32 Quantity) const
{
	InventoryComponent->ServerTryAddItemToSlot(BagIndex, SlotIndex, ItemID, Quantity);
}

void UProjectN_InventoryController::StackItems(const FName ItemID, const FGuid FromBagIndex, const FGuid ToBagIndex, const int32 FromSlotIndex, const int32 ToSlotIndex, const int32 QuantityToAdd)
{
	const FEntriesDefinition* EntriesDefinition = GetEntryManifest(ItemID);
	const FStackFragment* StackFragment = GetFragment<FStackFragment>(*EntriesDefinition->FragmentManifest, ProjectNGameplayTags::Fragment_Stack);

	InventoryComponent->ServerStackItems(FromBagIndex, ToBagIndex, FromSlotIndex, ToSlotIndex, QuantityToAdd, StackFragment->GetMaxStack());
}

void UProjectN_InventoryController::ReplaceItemsInBag(const FGuid FromBagIndex, const FGuid ToBagIndex,	const int32 FromSlotIndex, const int32 ToSlotIndex) const
{
	InventoryComponent->ServerReplaceItemInBag(FromBagIndex, ToBagIndex, FromSlotIndex, ToSlotIndex);
}

void UProjectN_InventoryController::RemoveItem(const FGuid FromBagIndex, const int32 FromSlotIndex) const
{
	InventoryComponent->ServerRemoveItem(FromBagIndex, FromSlotIndex);
}

/*********************************
*  Equipping manage
*********************************/
void UProjectN_InventoryController::EquipItemToSlot(const FName ItemID,	const EEquipSlot ToSlot, const int32 ItemStack) const
{
	InventoryComponent->ServerEquipItemToSlot(ItemID, ToSlot, ItemStack);
}

void UProjectN_InventoryController::UnEquipSlot(const EEquipSlot Slot) const
{
	InventoryComponent->ServerUnEquipSlot(Slot);
}

/*******************
*   Getters
********************/
FEntriesDefinition* UProjectN_InventoryController::GetEntryManifest(const FName& ItemID) const
{
	if (!Entries) return nullptr;
	
	const FString Context = FString(TEXT("UProjectN_InventoryController::FindEntryFromDataTable"));
	
	if (FEntriesDefinition* EntriesDefinition = Entries.LoadSynchronous()->FindRow<FEntriesDefinition>(ItemID, Context, false))
	{
		return EntriesDefinition;
	}
	return nullptr;
}