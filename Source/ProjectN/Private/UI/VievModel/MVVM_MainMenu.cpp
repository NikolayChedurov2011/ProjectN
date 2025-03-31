// N Chedurov All Rights Reserved


#include "UI/VievModel/MVVM_MainMenu.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_GameModeBase.h"
#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
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
		UCharacter_Save* SaveObj = LoadSlotData(Slot.Value->GetSlotName(), Slot.Key);

		Slot.Value->SlotStatus = SaveObj->SlotStatus;
		Slot.Value->GetPlayerName() = SaveObj->PlayerName;
		Slot.Value->AttributeInformation = SaveObj->GetSavedAttributes();
		Slot.Value->InitializeSlot();
	}
}

void UMVVM_MainMenu::NewGameSlotSelected(const int32 Index)
{
	SlotSelected.Broadcast(Index);
	
	CurrentSlotIndex = Index;
	//SaveSlotsMap[Index]->SlotStatus = ESaveSlotStatus::EnterName;
	SaveSlotsMap[Index]->InitializeSlot();
}

void UMVVM_MainMenu::NewSlotSaved(const int32 Index, const FString& CharacterName)
{
	const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(UGameplayStatics::GetPlayerState(this, 0));

	SaveSlotsMap[Index]->SlotStatus = ESaveSlotStatus::Taken;
	SaveSlotsMap[Index]->AttributeInformation = Cast<UProjectN_AbilitySystemComponent>(ASC)->GetAttributesForSave();
	SaveSlotsMap[Index]->SetPlayerName(CharacterName);
	SaveSlotsMap[Index]->InitializeSlot();
	SaveSlotData(SaveSlotsMap[Index], Index);
	
	SaveSlotsMap[Index]->SetSaveSlotState.Broadcast(1);
}

void UMVVM_MainMenu::DeleteSlot(const int32 Index)
{
	
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

void UMVVM_MainMenu::SaveSlotData(const UMVVM_SaveSlot* SaveSlotViewModel, const int32 SlotIndex) const
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotViewModel->GetSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotViewModel->GetSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
	UCharacter_Save* CharacterSave = Cast<UCharacter_Save>(SaveGameObject);
	CharacterSave->SetSavedAttributes(SaveSlotViewModel->AttributeInformation);
	CharacterSave->PlayerName = SaveSlotViewModel->GetPlayerName();
	CharacterSave->SlotStatus = ESaveSlotStatus::Taken;

	UGameplayStatics::SaveGameToSlot(CharacterSave, SaveSlotViewModel->GetSlotName(), SlotIndex);
}
