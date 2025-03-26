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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FProjectNAttributeSaveInfo> GetAttributesForSave() const;
	UFUNCTION(BlueprintCallable)
	void ChangeAttribute(const FGameplayTag& AttributeTag, float Value);

protected:

	UFUNCTION(Server, Reliable)
	void ServerCreateEffect(const FGameplayTag& AttributeTag, const float Value) const;
	
	void BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const;
	void CreateEffect(const FGameplayTag& AttributeTag, const float Value) const;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
