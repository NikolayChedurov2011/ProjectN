// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_ContainerWidgetBase.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_EquipmentWidget.generated.h"

class UProjectN_EntryDescription;
class UCanvasPanel;
class UProjectN_EntrySlotBase;
class UProjectN_EquipSlot;

UCLASS()
class PROJECTN_API UProjectN_EquipmentWidget : public UProjectN_ContainerWidgetBase
{
	GENERATED_BODY()

public:
	
	void ClearEquipmentSlot(const EEquipSlot SlotType);
	UProjectN_EquipSlot* FindEquipmentSlot(const EEquipSlot SlotType);

	UFUNCTION(BlueprintCallable)
	void BindToEquipSlotsEvent();
	
protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UProjectN_EquipSlot>> EquipmentSlots;
};