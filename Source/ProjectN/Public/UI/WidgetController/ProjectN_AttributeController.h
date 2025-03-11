// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_AttributeController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_AttributeController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;
};
