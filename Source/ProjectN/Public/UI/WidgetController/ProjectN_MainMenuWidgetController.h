// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_MainMenuWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttribute;
class UProjectN_WidgetBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotSelectedSignature, UUserWidget*, NewValue);

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_MainMenuWidgetController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSaveSlotSelectedSignature	OnSaveSlotSelected;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

protected:

	//void BindGameplayAttributeValueChange(const FGameplayAttribute& AttributeData, const FOnAttributeChangedSignature& OnAttributeChangedDelegate) const;
	
	template <typename T>
	T* GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag);
};

template <typename T>
T* UProjectN_MainMenuWidgetController::GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));
}
