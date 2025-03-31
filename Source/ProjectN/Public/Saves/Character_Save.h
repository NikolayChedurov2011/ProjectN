// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectN/ProjectNTypes.h"
#include "Character_Save.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

UCLASS()
class PROJECTN_API UCharacter_Save : public USaveGame
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Character Save")
	FORCEINLINE TArray<FProjectNAttributeSaveInfo>& GetSavedAttributes() { return SavedAttributes; }

	UFUNCTION(BlueprintCallable, Category = "Character Save")
	FORCEINLINE void SetSavedAttributes(const TArray<FProjectNAttributeSaveInfo>& AttributesToSave)
	{
		SavedAttributes.Empty();
		SavedAttributes = AttributesToSave;
	}
	
	UPROPERTY()
	FString PlayerName = FString();
		
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = ESaveSlotStatus::Vacant;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProjectNAttributeSaveInfo> SavedAttributes;
};