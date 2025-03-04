// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageRowSignature, FUIWidgetRow, WidgetRow);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedSignature, float, NewMaxStamina);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);

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

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> MessageWidgetDataTable; 

	void HealthChanged(const FOnAttributeChangeData& AttributeData) const;
	void MaxHealthChanged(const FOnAttributeChangeData& AttributeData) const;
	void ManaChanged(const FOnAttributeChangeData& AttributeData) const;
	void MaxManaChanged(const FOnAttributeChangeData& AttributeData) const;
	void StaminaChanged(const FOnAttributeChangeData& AttributeData) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& AttributeData) const;
	void BroadcastMessage(const FGameplayTagContainer& EffectAssetTags);

	template <typename T>
	T* GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag);
};

template <typename T>
T* UProjectN_OverlayWidgetController::GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));
}
