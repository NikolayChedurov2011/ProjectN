// N Chedurov All Rights Reserved


#include "UI/VievModel/MVVM_MainMenu.h"

#include "ProjectN_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/VievModel/MVVM_SaveSlot.h"

void UMVVM_MainMenu::InitializeSaveSlots()
{
	SaveSlot_0 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlot_0->SetSlotName(FString(TEXT("SaveSlot_0")));
	SaveSlot_0->SlotIndex = 0;
	SaveSlotsMap.Add(0, SaveSlot_0);

	SaveSlot_1 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlot_1->SetSlotName(FString(TEXT("SaveSlot_1")));
	SaveSlot_0->SlotIndex = 1;
	SaveSlotsMap.Add(1, SaveSlot_1);

	SaveSlot_2 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlot_2->SetSlotName(FString(TEXT("SaveSlot_2")));
	SaveSlot_0->SlotIndex = 2;
	SaveSlotsMap.Add(2, SaveSlot_2);

	SaveSlot_3 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlot_3->SetSlotName(FString(TEXT("SaveSlot_3")));
	SaveSlot_0->SlotIndex = 3;
	SaveSlotsMap.Add(3, SaveSlot_3);

	SaveSlot_4 = NewObject<UMVVM_SaveSlot>(this, SaveSlotViewModelClass);
	SaveSlot_4->SetSlotName(FString(TEXT("SaveSlot_4")));
	SaveSlot_0->SlotIndex = 4;
	SaveSlotsMap.Add(4, SaveSlot_4);
}

UMVVM_SaveSlot* UMVVM_MainMenu::GetSaveSlotViewModelByIndex(const int32 Index) const
{
	return SaveSlotsMap.FindChecked(Index);
}

void UMVVM_MainMenu::LoadData()
{	
	for (const TTuple<int, UMVVM_SaveSlot*> Slot : SaveSlotsMap)
	{
		// Load or create save object
		UCharacter_Save* SaveObj = LoadSlotData(Slot.Value->GetSlotName(), Slot.Key);

		// Write data from save object to view model
		Slot.Value->SlotStatus = SaveObj->SlotStatus;
		Slot.Value->GetPlayerName() = SaveObj->PlayerName;
		Slot.Value->Strength = SaveObj->Strength;
		Slot.Value->Intelligence = SaveObj->Intelligence;
		Slot.Value->Dexterity = SaveObj->Dexterity;
		Slot.Value->Vitality = SaveObj->Vitality;
		Slot.Value->InitializeSlot();
	}
}

UCharacter_Save* UMVVM_MainMenu::LoadSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		return Cast<UCharacter_Save>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	}
	else
	{
		return Cast<UCharacter_Save>(UGameplayStatics::CreateSaveGameObject(CharacterSaveClass));
	}
}

void UMVVM_MainMenu::NewGameSlotSelected(const int32 Index)
{
	CurrentSlotIndex = Index;
	SlotSelected.Broadcast(Index);
	Cast<AProjectN_PlayerCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn())->ApplyPrimaryAttributeFromSave(SaveSlotsMap[Index]->GetSlotName(), Index);
	//SaveSlotsMap[Index]->InitializeSlot();
}

void UMVVM_MainMenu::NewSlotSaved(const int32 Index, const FString& CharacterName)
{
	// Write data to view model
	SaveSlotsMap[Index]->SlotStatus = ESaveSlotStatus::Taken;
	SaveSlotsMap[Index]->SetPlayerName(CharacterName);
	
	Cast<AProjectN_PlayerCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn())->Save(SaveSlotsMap[Index]);

	SaveSlotsMap[Index]->SetSaveSlotState.Broadcast(1);
}

void UMVVM_MainMenu::DeleteSlot(const int32 Index)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotsMap[Index]->GetSlotName(), SaveSlotsMap[Index]->SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotsMap[Index]->GetSlotName(), SaveSlotsMap[Index]->SlotIndex);
	}
	SaveSlotsMap[Index]->ClearModel();
	SaveSlotsMap[Index]->InitializeSlot();
}