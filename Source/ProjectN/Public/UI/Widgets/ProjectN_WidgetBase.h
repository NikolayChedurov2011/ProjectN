// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectN_WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UProjectN_WidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

//	UFUNCTION(BlueprintCallable, BlueprintPure)
//	FORCEINLINE TObjectPtr<UObject> GetWidgetController() const { return WidgetController; }
	
protected:

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
};
