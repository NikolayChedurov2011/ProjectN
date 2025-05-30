// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_DragDropOperation.generated.h"

UCLASS()
class PROJECTN_API UProjectN_InventorySlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FProjectNInventorySlotInfo MyPayload;

	static UProjectN_InventorySlotDragDropOperation* New(const FProjectNInventorySlotInfo& InPayload)
	{
		UProjectN_InventorySlotDragDropOperation* Op = NewObject<UProjectN_InventorySlotDragDropOperation>();
		Op->MyPayload = InPayload;
		Op->Pivot  = EDragPivot::MouseDown;
		return Op;
	}
};

UCLASS()
class PROJECTN_API UProjectN_ActionSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FProjectNActionSlotInfo MyPayload;

	static UProjectN_ActionSlotDragDropOperation* New(const FProjectNActionSlotInfo& InPayload)
	{
		UProjectN_ActionSlotDragDropOperation* Op = NewObject<UProjectN_ActionSlotDragDropOperation>();
		Op->MyPayload = InPayload;
		Op->Pivot  = EDragPivot::MouseDown;
		return Op;
	}
};

/*
UCLASS()
class PROJECTN_API UProjectN_AbilitySlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FProjectNActionSlotInfo MyPayload;

	static UProjectN_AbilitySlotDragDropOperation* New(const FProjectNActionSlotInfo& InPayload)
	{
		UProjectN_AbilitySlotDragDropOperation* Op = NewObject<UProjectN_AbilitySlotDragDropOperation>();
		Op->MyPayload = InPayload;
		Op->Pivot  = EDragPivot::MouseDown;
		return Op;
	}
};
*/
