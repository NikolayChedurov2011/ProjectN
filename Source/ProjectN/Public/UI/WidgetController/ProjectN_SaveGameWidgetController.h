// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_SaveGameWidgetController.generated.h"

class USaveGame;
class UCharacter_Save;
struct FProjectNAttributeSaveInfo;
struct FGameplayTag;
struct FGameplayAttribute;
class UAttributeInfo;

USTRUCT()
struct FCurrentGameSlot
{
	GENERATED_BODY()

	FString SlotIndex;

	UPROPERTY()
	UCharacter_Save* SaveGameObj = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveGameLoadedSignature, const TArray<FProjectNAttributeSaveInfo>&, AttributeInfo, const FString&, Slot);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_SaveGameWidgetController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Attribute Menu")
	FOnSaveGameLoadedSignature AttributeInfoDelegate;
	
	virtual void BroadcastInitialValues() override;
	
	UFUNCTION(BlueprintCallable)
	void SaveAttributes(const TArray<FProjectNAttributeSaveInfo> AttributesInfo, const FString& Slot) const;

	UFUNCTION(BlueprintCallable)
	void DeleteSave(const FString& Slot) const;

protected:

	USaveGame* LoadOrCreateSaveGame(const FString& Slot) const;
	
	UPROPERTY()
	FCurrentGameSlot CurrentGameSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCharacter_Save> CharacterSaveSubClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxSaveSlots = 5;
};
