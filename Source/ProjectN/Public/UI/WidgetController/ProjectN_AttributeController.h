// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_AttributeController.generated.h"

struct FProjectNAttributeSaveInfo;
struct FProjectNAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;
class UAttributeInfo;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegateSignature, const FProjectNAttributeInfo&, AttributeInfo);

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_AttributeController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Attribute Menu")
	FAttributeInfoDelegateSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="Gameplay Values")
	FOnCharacterStatChangedSignature OnAttributePointsChanged;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;
	
	UFUNCTION(BlueprintCallable)
	void ChangeAttribute(const FGameplayTag& AttributeTag, const float Value);

protected:
	
	void BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
