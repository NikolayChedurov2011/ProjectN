// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectN_DragDropItem.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECTN_API UProjectN_DragDropItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	void SetItemIcon(const FSlateBrush& IconBrush) const;
	void SetStackCount(const int32 NewStackCount) const;

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackCount;
};
