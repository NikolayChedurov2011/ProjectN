// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_MainMenu.generated.h"

class USaveGame;
class UCharacter_Save;
class UMVVM_SaveSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotSelectedSignature, int32, SlotIndex);

UCLASS()
class PROJECTN_API UMVVM_MainMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSlotSelectedSignature SlotSelected;
	
	void InitializeSaveSlots();

	UFUNCTION(BlueprintPure)
	UMVVM_SaveSlot* GetSaveSlotViewModelByIndex(const int32 Index) const;
	
	UFUNCTION(BlueprintCallable)
	void NewGameSlotSelected(const int32 Index);

	UFUNCTION(BlueprintCallable)
	void NewSlotSaved(const int32 Index, const FString& CharacterName);

	UFUNCTION(BlueprintCallable)
	void DeleteSlot(const int32 Index);

	UFUNCTION(BlueprintCallable)
	void LoadData();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_SaveSlot> SaveSlotViewModelClass;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentSlotIndex = -1;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USaveGame> CharacterSaveClass;
	
private:

	UCharacter_Save* LoadSlotData(const FString& SlotName, const int32 SlotIndex) const;

	UPROPERTY()
	TMap<int32, UMVVM_SaveSlot*> SaveSlotsMap;

	UPROPERTY()
	TObjectPtr<UMVVM_SaveSlot> SaveSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_SaveSlot> SaveSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_SaveSlot> SaveSlot_2;

	UPROPERTY()
	TObjectPtr<UMVVM_SaveSlot> SaveSlot_3;

	UPROPERTY()
	TObjectPtr<UMVVM_SaveSlot> SaveSlot_4;
};