// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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
	
	UPROPERTY()
	FString PlayerName = FString();
		
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = ESaveSlotStatus::Vacant;
	
	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Dexterity = 0.f;

	UPROPERTY()
	float Vitality = 0.f;
	
protected:
};