// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_InventoryWidget.generated.h"

struct FBagData;
class UProjectN_InventorySlot;
class UWrapBox;

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

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> InventoryGrid;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UProjectN_InventorySlot>> InventorySlots;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UProjectN_InventorySlot> InventorySlotClass;
};
