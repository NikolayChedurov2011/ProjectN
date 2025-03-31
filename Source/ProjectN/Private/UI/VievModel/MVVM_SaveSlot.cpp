// N Chedurov All Rights Reserved


#include "UI/VievModel/MVVM_SaveSlot.h"

void UMVVM_SaveSlot::InitializeSlot()
{
	SetSaveSlotState.Broadcast(SlotStatus.GetValue());
}

void UMVVM_SaveSlot::SetPlayerName(const FString& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_SaveSlot::SetSlotName(const FString& InSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotName, InSlotName);
}
