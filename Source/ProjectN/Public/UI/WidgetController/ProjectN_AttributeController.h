// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_AttributeController.generated.h"

struct FProjectNAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegateSignature, const FProjectNAttributeInfo&, AttributeInfo);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_AttributeController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Attribute Menu")
	FAttributeInfoDelegateSignature AttributeInfoDelegate;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

protected:

	void BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
