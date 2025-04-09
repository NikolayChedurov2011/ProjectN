// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Saves/Character_Save.h"
#include "MVVM_SaveSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetSaveSlotStateSignature, int32, Index);

UCLASS()
class PROJECTN_API UMVVM_SaveSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSetSaveSlotStateSignature SetSaveSlotState;

	void InitializeSlot();

	// Save slot data
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ESaveSlotStatus> SlotStatus = ESaveSlotStatus::Vacant;
	
	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	FString SlotName = FString();

	// Player data
	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 SkillTreePoints = 0;
	
	/*UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;

	UPROPERTY()
	float Dexterity = 0.f;

	UPROPERTY()
	float Vitality = 0.f;*/

	void SetPlayerName(const FString& InPlayerName);
	FString GetPlayerName() const { return PlayerName; }

	void SetLevel(const int32 NewLevel);
	int32 GetLevel() const { return Level; }

	void ClearModel();

private:
	
	/* Field notifies */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	int32 Level;
};
