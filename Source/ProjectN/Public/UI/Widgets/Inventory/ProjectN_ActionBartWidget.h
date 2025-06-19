// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_ActionBartWidget.generated.h"

class UProjectN_EntrySlotBase;
class UCanvasPanel;
class UProjectN_EntryDescription;
class UProjectN_ActionSlot;

UCLASS()
class PROJECTN_API UProjectN_ActionBartWidget : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:
	
	void ClearActionSlot(const int32 SlotIndex);
	void ClearActionSlot(const FGameplayTag& InputTag);
	UProjectN_ActionSlot* ActionSlot(const int32 SlotIndex);
	UProjectN_ActionSlot* ActionSlot(const FGameplayTag& InputTag);

	FORCEINLINE TArray<UProjectN_ActionSlot*> GetActionSlots() const { return ActionSlots; }

	UFUNCTION(BlueprintCallable)
	void BindToActionSlotsEvent();
	
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
	TArray<TObjectPtr<UProjectN_ActionSlot>> ActionSlots;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, Category="Description")
	TSubclassOf<UProjectN_EntryDescription> DescriptionWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UProjectN_EntryDescription> DescriptionWidget = nullptr;

	FTimerHandle DescriptionTimerHandle;
	float DescriptionTimerShowAt = 0.1;
};