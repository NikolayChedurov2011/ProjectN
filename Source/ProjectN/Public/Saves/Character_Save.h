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

	// Save slot data
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = ESaveSlotStatus::Vacant;
		
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;

	// Player data
	UPROPERTY()
	FString PlayerName = FString();
	
	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;
	
	UPROPERTY()
	float Strength = 1.f;

	UPROPERTY()
	float Intelligence = 1.f;

	UPROPERTY()
	float Dexterity = 1.f;

	UPROPERTY()
	float Vitality = 1.f;
	
protected:
};