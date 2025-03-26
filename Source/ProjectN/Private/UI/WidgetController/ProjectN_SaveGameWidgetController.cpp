// N Chedurov All Rights Reserved


#include "UI/WidgetController/ProjectN_SaveGameWidgetController.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/Character_Save.h"

void UProjectN_SaveGameWidgetController::BroadcastInitialValues()
{
	if (!IsValid(AttributeSet) || !IsValid(AbilitySystemComponent))
	{
		return;
	}
}

void UProjectN_SaveGameWidgetController::RememberCurrentGameSlot(const FString& InSlot)
{
	if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(InSlot, 0))
	{
		CurrentGameSlot.SlotIndex = InSlot;
		CurrentGameSlot.SaveGameObj = Cast<UCharacter_Save>(SaveGame);
	}
	else
	{
		if (USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(CharacterSaveSubClass))
		{
			CurrentGameSlot.SlotIndex = InSlot;
			CurrentGameSlot.SaveGameObj = Cast<UCharacter_Save>(NewSaveGame);
		}
	}
}

void UProjectN_SaveGameWidgetController::LoadSaveGame() const
{
	for (int32 SlotId = 0; SlotId < 5; SlotId++)
	{
		 FString SlotName = FString::Printf(TEXT("AttributesSave%d"), SlotId);
		 if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0))
		 {
		 	UCharacter_Save* SaveGameObject = Cast<UCharacter_Save>(SaveGame);
		 	AttributeInfoDelegate.Broadcast(SaveGameObject->GetSavedAttributes(), SlotId);
		 }
	}
}

void UProjectN_SaveGameWidgetController::SaveAttributes(const TArray<FProjectNAttributeSaveInfo> AttributesInfo) const
{
	CurrentGameSlot.SaveGameObj->SetSavedAttributes(AttributesInfo);
	UGameplayStatics::SaveGameToSlot(CurrentGameSlot.SaveGameObj, CurrentGameSlot.SlotIndex, 0);
}
