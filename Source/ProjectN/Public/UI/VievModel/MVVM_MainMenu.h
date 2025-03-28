// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_MainMenu.generated.h"

class UMVVM_SaveSlot;

UCLASS()
class PROJECTN_API UMVVM_MainMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeSaveSlots();

	UFUNCTION(BlueprintPure)
	UMVVM_SaveSlot* GetSaveSlotViewModelByIndex(const int32 Index) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_SaveSlot> SaveSlotViewModelClass;

	UFUNCTION(BlueprintCallable)
	void NewGameSlotSelected(const int32 Index);

	UFUNCTION(BlueprintCallable)
	void NewSlotSaved(const int32 Index, const FString& CharacterName);
	
private:

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