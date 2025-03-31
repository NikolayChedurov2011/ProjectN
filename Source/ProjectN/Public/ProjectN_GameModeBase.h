// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectN_GameModeBase.generated.h"

class UCharacter_Save;
class USaveGame;
class UMVVM_SaveSlot;

UCLASS(Abstract)
class PROJECTN_API AProjectN_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void SaveSlotData(UMVVM_SaveSlot* SaveSlotViewModel, const int32 SlotIndex) const;
	UCharacter_Save* LoadSlotData(const FString& SlotName, const int32 SlotIndex) const;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USaveGame> CharacterSaveClass;
};