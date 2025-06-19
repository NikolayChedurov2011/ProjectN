// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_EquipmentWidget.generated.h"

class UProjectN_EquipSlot;

UCLASS()
class PROJECTN_API UProjectN_EquipmentWidget : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:
	
	void ClearEquipmentSlot(const EEquipSlot SlotType);
	UProjectN_EquipSlot* FindEquipmentSlot(const EEquipSlot SlotType);
	
protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UProjectN_EquipSlot>> EquipmentSlots;
};