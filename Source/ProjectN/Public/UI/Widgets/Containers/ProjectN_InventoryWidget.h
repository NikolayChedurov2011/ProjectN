// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_ContainerWidgetBase.h"
#include "ProjectN_InventoryWidget.generated.h"

struct FBagData;
class UProjectN_InventorySlot;
class UWrapBox;

UCLASS()
class PROJECTN_API UProjectN_InventoryWidget : public UProjectN_ContainerWidgetBase
{
	GENERATED_BODY()

public:
	
	void InitNewBag(const FGuid BagIndex, const int32 SlotIndex);
	//void RemoveBag(const FBagData& NewBagData);
	bool IsBagExist(const FGuid BagIndex);
	UProjectN_InventorySlot* FindInventorySlot(const FGuid BagIndex, const int32 BagSlotIndex);
	void ClearInventorySlot(const FGuid BagIndex, const int32 BagSlotIndex);

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
