// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttribute;
class UProjectN_WidgetBase;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UProjectN_WidgetBase> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignature, float, NewValue, float, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageRowSignature, FUIWidgetRow, WidgetRow);

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_OverlayWidgetController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature	OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature	OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature	OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature	OnManaChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature	OnMaxStaminaChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature	OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMessageRowSignature			OnMessageRowSignature;
	
	UPROPERTY(BlueprintAssignable, Category="Gameplay Values")
	FOnCharacterStatChangedSignature OnXPChanged;

	UPROPERTY(BlueprintAssignable, Category="Gameplay Values")
	FOnCharacterStatChangedSignature OnLevelChanged;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> MessageWidgetDataTable; 

	void BindGameplayAttributeValueChange(const FGameplayAttribute& AttributeData, const FOnAttributeChangedSignature& OnAttributeChangedDelegate) const;
	
	template <typename T>
	T* GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag);
};

template <typename T>
T* UProjectN_OverlayWidgetController::GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));
}
