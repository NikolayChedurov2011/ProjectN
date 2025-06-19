// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_OverlayWidgetController.generated.h"

struct FActionSlotData;
struct FGameplayAttribute;
struct FOnAttributeChangeData;
class UProjectN_AbilitySystemComponent;
class UAbilityInfo;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCooldownChangeSignature, const FGameplayTag, CooldownTag, const float, RemainingValue);

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

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature		CooldownChange;
	
	UPROPERTY(BlueprintAssignable, Category="Gameplay Values")
	FOnCharacterStatChangedSignature OnXPChanged;

	UPROPERTY(BlueprintAssignable, Category="Gameplay Values")
	FOnCharacterStatChangedSignature OnLevelChanged;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

	UFUNCTION(BlueprintCallable)
	float CheckCooldownRemainig(const FGameplayTag& CooldownTag) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfoDataAsset;

	void BindGameplayAttributeValueChange(const FGameplayAttribute& AttributeData, const FOnAttributeChangedSignature& OnAttributeChangedDelegate) const;

	void ProcessXP(const int32 Value) const;
	void OnInitializeAbility(UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent) const;
	
	template <typename T>
	T* GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag);
};

template <typename T>
T* UProjectN_OverlayWidgetController::GetTableRowByTag(UDataTable* DataTable, FGameplayTag GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(), TEXT(""));
}
