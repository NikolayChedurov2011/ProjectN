// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_AbilityBookController.generated.h"

class UProjectN_AbilityBookWidget;
class UProjectN_ActionBartWidget;

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_AbilityBookController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

	UFUNCTION(BlueprintCallable)
	void AddAbility(const FName& AbilityID) const;

	UFUNCTION(BlueprintCallable)
	void SetAbilityBookWidgetRef(UProjectN_AbilityBookWidget* NewAbilityBookWidget);
	
/*******************
*   Getters
********************/
	FEntriesDefinition* GetEntryManifest(const FName& ItemID) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Items Data Table")
	TObjectPtr<UDataTable> Entries;

private:

	UPROPERTY()
	TObjectPtr<UProjectN_AbilityBookWidget> AbilityBookWidget;
};
