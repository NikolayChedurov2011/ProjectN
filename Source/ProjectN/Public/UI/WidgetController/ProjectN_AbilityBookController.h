// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_AbilityBookController.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateAbilitySlotSignature, const FAbilitySlotData, AbilitySlotInfo);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_AbilityBookController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Ability Slot Info")
	FOnUpdateAbilitySlotSignature OnUpdateAbilitySlot;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

	UFUNCTION(BlueprintCallable)
	void AddAbility(const FName& AbilityID);
	
protected:

	void LoadAbilityIcon(FAbilitySlotData& ActionSlotInfo) const;
	
	const FAbilityDefinition* GetAbilityData(const FName& ItemID) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TSoftObjectPtr<UDataTable> AbilityDataTable;

private:
	TArray<FAbilitySlotData> Abilities;
};
