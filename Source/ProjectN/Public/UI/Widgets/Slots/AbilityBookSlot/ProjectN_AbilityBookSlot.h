// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_AbilityBookSlot.generated.h"

class UProjectN_DragDropItem;

UCLASS()
class PROJECTN_API UProjectN_AbilityBookSlot : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:
	UProjectN_AbilityBookSlot() {}
	
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	FORCEINLINE FSlateBrush GetIconBrush() const { return AbilityIcon->GetBrush(); }
	FORCEINLINE void SetAbilityIcon(const FSlateBrush& IconBrush) const;

	FORCEINLINE FName GetItemID() const { return ItemID; }
	FORCEINLINE void SetItemID(const FName& NewItemID) { ItemID = NewItemID; }

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AbilityIcon;
	
	UPROPERTY(EditDefaultsOnly, Category="Drag Drop")
	TSubclassOf<UProjectN_DragDropItem> DragDropWidgetClass = nullptr;
	
	FName ItemID = NAME_None;
};