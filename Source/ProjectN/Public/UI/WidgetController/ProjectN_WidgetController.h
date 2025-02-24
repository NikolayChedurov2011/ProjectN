// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_WidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class PROJECTN_API UProjectN_WidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UProjectN_WidgetController();

protected:

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Widget Controller")
	TObjectPtr<APlayerController> PlayerController;
};
