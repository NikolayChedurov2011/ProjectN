// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_InventoryWidget.generated.h"

class UProjectN_EntrySlotBase;
struct FBagData;
class UProjectN_InventorySlot;
class UWrapBox;
class UProjectN_EntryDescription;
class UCanvasPanel;

UCLASS()
class PROJECTN_API UProjectN_InventoryWidget : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:
	
	void InitNewBag(const FBagData& NewBagData);
	void RemoveBag(const FBagData& NewBagData);
	bool IsBagExist(const int32 BagIndex);
	UProjectN_InventorySlot* FindInventorySlot(const int32 BagIndex, const int32 BagSlotIndex);
	void ClearInventorySlot(const int32 BagIndex, const int32 BagSlotIndex);

	bool IsRightMouseClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftMouseClick(const FPointerEvent& MouseEvent) const;

protected:

	////////
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
	TObjectPtr<UWrapBox> InventoryGrid;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UProjectN_InventorySlot>> InventorySlots;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UProjectN_InventorySlot> InventorySlotClass;

		/////
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, Category="Description")
	TSubclassOf<UProjectN_EntryDescription> DescriptionWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UProjectN_EntryDescription> DescriptionWidget = nullptr;

	FTimerHandle DescriptionTimerHandle;
	float DescriptionTimerShowAt = 0.1;
};
