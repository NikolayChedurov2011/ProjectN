// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_AbilityBookWidget.generated.h"

class UGridPanel;
class UProjectN_AbilityBookSlot;
class UWrapBox;

UCLASS()
class PROJECTN_API UProjectN_AbilityBookWidget : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:

	UProjectN_AbilityBookSlot* FindAbility(const FName& AbilityID);
	UProjectN_AbilityBookSlot* AddNewAbility();

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGridPanel> AbilityBookGrid;

	UPROPERTY()
	TArray<TObjectPtr<UProjectN_AbilityBookSlot>> AbilitySlots;
	
	UPROPERTY(EditDefaultsOnly, Category="Default Params")
	TSubclassOf<UProjectN_AbilityBookSlot> AbilitySlotClass;

	UPROPERTY(EditInstanceOnly, Category="Default Params")
	int32 Columns = 2;
};