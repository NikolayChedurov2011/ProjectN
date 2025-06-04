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
	FInventorySlotData MyPayload;

	static UProjectN_InventorySlotDragDropOperation* New(const FInventorySlotData& InPayload)
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
	FActionSlotData MyPayload;

	static UProjectN_ActionSlotDragDropOperation* New(const FActionSlotData& InPayload)
	{
		UProjectN_ActionSlotDragDropOperation* Op = NewObject<UProjectN_ActionSlotDragDropOperation>();
		Op->MyPayload = InPayload;
		Op->Pivot  = EDragPivot::MouseDown;
		return Op;
	}
};

UCLASS()
class PROJECTN_API UProjectN_EquipSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FEquipSlotData MyPayload;

	static UProjectN_EquipSlotDragDropOperation* New(const FEquipSlotData& InPayload)
	{
		UProjectN_EquipSlotDragDropOperation* Op = NewObject<UProjectN_EquipSlotDragDropOperation>();
		Op->MyPayload = InPayload;
		Op->Pivot  = EDragPivot::MouseDown;
		return Op;
	}
};

UCLASS()
class PROJECTN_API UProjectN_AbilitySlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FAbilitySlotData MyPayload;

	static UProjectN_AbilitySlotDragDropOperation* New(const FAbilitySlotData& InPayload)
	{
		UProjectN_AbilitySlotDragDropOperation* Op = NewObject<UProjectN_AbilitySlotDragDropOperation>();
		Op->MyPayload = InPayload;
		Op->Pivot  = EDragPivot::MouseDown;
		return Op;
	}
};