// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "GameFramework/SaveGame.h"
#include "Character_Save.generated.h"

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
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProjectNAttributeSaveInfo> SavedAttributes;
};