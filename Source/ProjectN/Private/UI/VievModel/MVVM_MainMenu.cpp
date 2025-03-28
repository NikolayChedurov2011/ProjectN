// N Chedurov All Rights Reserved


#include "UI/VievModel/MVVM_MainMenu.h"

#include "UI/VievModel/MVVM_SaveSlot.h"

void UMVVM_MainMenu::InitializeSaveSlots()
{
	SaveSlot_0 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlotsMap.Add(0, SaveSlot_0);

	SaveSlot_1 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlotsMap.Add(1, SaveSlot_1);

	SaveSlot_2 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlotsMap.Add(2, SaveSlot_2);

	SaveSlot_3 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlotsMap.Add(3, SaveSlot_3);

	SaveSlot_4 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlotsMap.Add(4, SaveSlot_4);
}

UMVVM_SaveSlot* UMVVM_MainMenu::GetSaveSlotViewModelByIndex(const int32 Index) const
{
	return SaveSlotsMap.FindChecked(Index);
}

void UMVVM_MainMenu::NewGameSlotSelected(const int32 Index)
{
	SaveSlotsMap[Index]->SetSaveSlotState.Broadcast(0);
}

void UMVVM_MainMenu::NewSlotSaved(const int32 Index, const FString& CharacterName)
{
	SaveSlotsMap[Index]->SetSaveSlotState.Broadcast(1);
}
