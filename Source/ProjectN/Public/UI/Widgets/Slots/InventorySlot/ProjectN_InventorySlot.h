// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "UI/Widgets/Slots/ProjectN_EntrySlotBase.h"
#include "ProjectN_InventorySlot.generated.h"

class UProgressBar;
class UProjectN_DragDropItem;
class UTextBlock;

UCLASS()
class PROJECTN_API UProjectN_InventorySlot : public UProjectN_EntrySlotBase
{
	GENERATED_BODY()

public:
	UProjectN_InventorySlot() {}
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	FORCEINLINE FSlateBrush GetIconBrush() const { return ItemIcon->GetBrush(); }
	void SetItemIcon(const FSlateBrush& IconBrush) const;

	FORCEINLINE FGuid GetBagIndex() const { return BagIndex; }
	FORCEINLINE void SetBagIndex(const FGuid InBagID) { BagIndex = InBagID; }
	
	FORCEINLINE int32 GetBagSlotIndex() const { return BagSlotIndex; }
	FORCEINLINE void SetBagSlotIndex(const int32 InBagSlotIndex) { BagSlotIndex = InBagSlotIndex; }
	
	FORCEINLINE FName GetItemID() const { return ItemID; }
	FORCEINLINE void SetItemID(const FName& NewItemID) { ItemID = NewItemID; }
	
	FORCEINLINE int32 GetStackCount() const { return Stack; }
	void SetStackCountText(const int32 NewStackCountText) const;
	FORCEINLINE void SetStackCount(const int32 NewStackCount) { Stack =  NewStackCount; SetStackCountText(Stack); }

	FORCEINLINE void SetCooldownTag(const FGameplayTag& NewCooldownTag) { CooldownTag = NewCooldownTag; }
	FORCEINLINE void SetCooldownBaseValue(const float NewBaseCooldownValue) { BaseCooldownValue = NewBaseCooldownValue; }
	FORCEINLINE void SetCooldownValueRemaining(const float NewCooldownValueRemaining) { CooldownValueRemaining = NewCooldownValueRemaining; SetCooldownText(CooldownValueRemaining); }

	UFUNCTION(BlueprintCallable)
	void SetCooldownText(const float NewCooldownValueRemaining) const;
	
	UFUNCTION(BlueprintCallable)
	void UpdateCooldownProgressBar(const float NewCooldownValueRemaining) const;
	
	void ClearSlot();

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackCount;

	UPROPERTY(EditDefaultsOnly, Category="Drag Drop")
	TSubclassOf<UProjectN_DragDropItem> DragDropWidgetClass = nullptr;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> CooldownProgressBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CooldownText;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float BaseCooldownValue = 0.f;
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float CooldownValueRemaining = 0.f;
	
	FGuid BagIndex = FGuid();
	int32 BagSlotIndex = INDEX_NONE;
	int32 Stack = 0;
};