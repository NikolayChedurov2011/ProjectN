// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_WidgetBase.h"
#include "ProjectN_DragDropItemBase.generated.h"

UCLASS()
class PROJECTN_API UProjectN_DragDropItemBase : public UProjectN_WidgetBase
{
	GENERATED_BODY()

protected:
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent );
};
