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

void UMVVM_SaveSlot::SetLevel(const int32 NewLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(Level, NewLevel);
}

void UMVVM_SaveSlot::ClearModel()
{
	SlotStatus = ESaveSlotStatus::Vacant;

	SetLevel(0);
	XP = 0;
	AttributePoints = 0;
	Strength = 0.f;
	Intelligence = 0.f;
	Dexterity = 0.f;
	Vitality = 0.f;
	SetPlayerName("");
}
