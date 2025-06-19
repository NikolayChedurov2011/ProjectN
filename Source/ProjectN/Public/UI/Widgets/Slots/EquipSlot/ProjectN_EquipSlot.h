// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "ProjectN/ProjectNTypes.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_EquipSlot.generated.h"

class UProjectN_DragDropItem;
class UTextBlock;

UCLASS()
class PROJECTN_API UProjectN_EquipSlot : public UProjectN_WidgetBase
{
	GENERATED_BODY()

public:
	UProjectN_EquipSlot() {}
	
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	FORCEINLINE FSlateBrush GetIconBrush() const { return ItemIcon->GetBrush(); }
	FORCEINLINE void SetItemIcon(const FSlateBrush& IconBrush) const;

	FORCEINLINE EEquipSlot GetEquipSlot() const { return EquipSlot; }
	
	FORCEINLINE FName GetItemID() const { return ItemID; }
	FORCEINLINE void SetItemID(const FName& NewItemID) { ItemID = NewItemID; }
	
	void ClearSlot();

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(EditInstanceOnly, Category="Default Params")
	EEquipSlot EquipSlot = EEquipSlot::None;

	UPROPERTY(EditInstanceOnly, Category="Default Params")
	TObjectPtr<UTexture2D> DefaultSlotIcon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Drag Drop")
	TSubclassOf<UProjectN_DragDropItem> DragDropWidgetClass = nullptr;
	
	FName ItemID = NAME_None;
};