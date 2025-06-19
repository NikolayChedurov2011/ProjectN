// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_AbilityBookWidget.generated.h"

class UProjectN_EntrySlotBase;
class UCanvasPanel;
class UProjectN_EntryDescription;
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

	UProjectN_EntryDescription* GetItemDescription();
	void SetDescriptionSizeAndPosition(UProjectN_EntrySlotBase* HoveredWidget);
	FVector2D GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, UProjectN_EntrySlotBase* HoveredWidget);
	FVector2D GetWidgetPosition(UWidget* UserWidget);
	FVector2D GetWidgetSize(UWidget* UserWidget);
	
	UFUNCTION()
	void OnSlotHovered(UProjectN_EntrySlotBase* HoveredWidget);
	
	UFUNCTION()
	void OnSlotUnhovered();

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGridPanel> AbilityBookGrid;

	UPROPERTY()
	TArray<TObjectPtr<UProjectN_AbilityBookSlot>> AbilitySlots;
	
	UPROPERTY(EditDefaultsOnly, Category="Default Params")
	TSubclassOf<UProjectN_AbilityBookSlot> AbilitySlotClass;

	UPROPERTY(EditInstanceOnly, Category="Default Params")
	int32 Columns = 2;
		
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, Category="Description")
	TSubclassOf<UProjectN_EntryDescription> DescriptionWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UProjectN_EntryDescription> DescriptionWidget = nullptr;

	FTimerHandle DescriptionTimerHandle;
	float DescriptionTimerShowAt = 0.1;
};