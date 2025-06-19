// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_ActionBartWidget.generated.h"

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
	
protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UProjectN_ActionSlot>> ActionSlots;
};