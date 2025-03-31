// N Chedurov All Rights Reserved


#include "ProjectN_GameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Saves/Character_Save.h"
#include "UI/VievModel/MVVM_SaveSlot.h"

UCharacter_Save* AProjectN_GameModeBase::LoadSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	//USaveGame* SaveGameObj = nullptr;
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		return Cast<UCharacter_Save>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	}
	else
	{
		return Cast<UCharacter_Save>(UGameplayStatics::CreateSaveGameObject(CharacterSaveClass));
	}

	//return Cast<UCharacter_Save>(SaveGameObj);
}

void AProjectN_GameModeBase::SaveSlotData(UMVVM_SaveSlot* SaveSlotViewModel, const int32 SlotIndex) const
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotViewModel->GetSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotViewModel->GetSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
	UCharacter_Save* CharacterSave = Cast<UCharacter_Save>(SaveGameObject);
	CharacterSave->PlayerName = SaveSlotViewModel->GetPlayerName();
	CharacterSave->SlotStatus = ESaveSlotStatus::Taken;

	UGameplayStatics::SaveGameToSlot(CharacterSave, SaveSlotViewModel->GetSlotName(), SlotIndex);
}
