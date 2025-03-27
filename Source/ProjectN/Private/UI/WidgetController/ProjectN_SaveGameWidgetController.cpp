// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_SaveGameWidgetController.h"

#include "Kismet/GameplayStatics.h"
#include "Saves/Character_Save.h"

void UProjectN_SaveGameWidgetController::BroadcastInitialValues()
{	
	for (int32 SlotId = 0; SlotId < MaxSaveSlots; SlotId++)
	{
		FString SlotName = FString::Printf(TEXT("AttributesSave%d"), SlotId);
		if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0))
		{
			UCharacter_Save* SaveGameObject = Cast<UCharacter_Save>(SaveGame);
			AttributeInfoDelegate.Broadcast(SaveGameObject->GetSavedAttributes(), SlotName);
		}
	}
}

void UProjectN_SaveGameWidgetController::SaveAttributes(const TArray<FProjectNAttributeSaveInfo> AttributesInfo, const FString& Slot) const
{
	USaveGame* SaveGame = LoadOrCreateSaveGame(Slot);
	
	UCharacter_Save* SaveGameObject = Cast<UCharacter_Save>(SaveGame);
	SaveGameObject->SetSavedAttributes(AttributesInfo);
	
	UGameplayStatics::SaveGameToSlot(SaveGameObject, Slot, 0);

	AttributeInfoDelegate.Broadcast(AttributesInfo, Slot);
}

void UProjectN_SaveGameWidgetController::DeleteSave(const FString& Slot) const
{	
	UGameplayStatics::DeleteGameInSlot(Slot, 0);

	TArray<FProjectNAttributeSaveInfo> EmptyAttributeInfo;
	AttributeInfoDelegate.Broadcast(EmptyAttributeInfo, Slot);
}

USaveGame* UProjectN_SaveGameWidgetController::LoadOrCreateSaveGame(const FString& Slot) const
{
	checkf(CharacterSaveSubClass, TEXT("Please fill out the character save sub class in BP_SaveGameWidgetController"))
	
	if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(Slot, 0))
	{
		return SaveGame;
	}
	
	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(CharacterSaveSubClass);
	return NewSaveGame;
}