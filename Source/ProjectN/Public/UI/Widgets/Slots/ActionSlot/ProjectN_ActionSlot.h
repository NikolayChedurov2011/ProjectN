// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "UI/Widgets/ProjectN_WidgetBase.h"
#include "ProjectN_ActionSlot.generated.h"

class UProgressBar;
class UTextBlock;
class UProjectN_DragDropItem;
class UProjectN_ActionSlot;

UCLASS()
class PROJECTN_API UProjectN_ActionSlot : public UProjectN_WidgetBase
{
	GENERATED_BODY()
	
public:	
	UProjectN_ActionSlot() {}

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
	FORCEINLINE FGameplayTag GetInputTag() const { return InputTag; }
	
	FORCEINLINE FSlateBrush GetIconBrush() const { return ActionSlotImage->GetBrush(); }
	FORCEINLINE void SetItemIcon(const FSlateBrush& IconBrush) const;

	FORCEINLINE void SetCostText(const int32 CostValue) const;
	
	FORCEINLINE FName GetItemID() const { return ItemID; }
	FORCEINLINE void SetItemID(const FName& NewItemID) { ItemID = NewItemID; }

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
	TObjectPtr<UImage> ActionSlotImage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CostText;

	UPROPERTY(EditInstanceOnly, Category="Default Params")
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(EditInstanceOnly, Category="Default Params")
	FGameplayTag InputTag = FGameplayTag();
	
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
	
	FName ItemID = NAME_None;
};
