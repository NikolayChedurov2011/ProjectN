// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_EquipmentWidget.generated.h"

class UProjectN_EntryDescription;
class UCanvasPanel;
class UProjectN_EntrySlotBase;
class UProjectN_EquipSlot;

UCLASS()
class PROJECTN_API UProjectN_EquipmentWidget : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:
	
	void ClearEquipmentSlot(const EEquipSlot SlotType);
	UProjectN_EquipSlot* FindEquipmentSlot(const EEquipSlot SlotType);

	UFUNCTION(BlueprintCallable)
	void BindToEquipSlotsEvent();
	
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

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UProjectN_EquipSlot>> EquipmentSlots;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, Category="Description")
	TSubclassOf<UProjectN_EntryDescription> DescriptionWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UProjectN_EntryDescription> DescriptionWidget = nullptr;

	FTimerHandle DescriptionTimerHandle;
	float DescriptionTimerShowAt = 0.1;
};