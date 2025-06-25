// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_EntrySlotBase.generated.h"

class USizeBox;
class UProjectN_EntryDescription;
class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotHovered, UProjectN_EntrySlotBase*, HoveredWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotUnhovered);

UCLASS()
class PROJECTN_API UProjectN_EntrySlotBase : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:

	FOnSlotHovered OnSlotHovered;
	FOnSlotUnhovered OnSlotUnhovered;

	FName GetEntryID() const { return ItemID; }
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	
	FName ItemID = NAME_None;
};
