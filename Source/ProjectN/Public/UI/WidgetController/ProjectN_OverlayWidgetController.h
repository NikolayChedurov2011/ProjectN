// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"
#include "ProjectN_OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedSignature, float, NewMaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_OverlayWidgetController : public UProjectN_WidgetControllerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChangedSignature	OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature		OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnMaxManaChangedSignature		OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnMaxManaChangedSignature		OnManaChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnMaxStaminaChangedSignature	OnMaxStaminaChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChangedSignature		OnStaminaChanged;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToResponce() override;

protected:

	void HealthChanged(const FOnAttributeChangeData& AttributeData) const;
	void MaxHealthChanged(const FOnAttributeChangeData& AttributeData) const;
	void ManaChanged(const FOnAttributeChangeData& AttributeData) const;
	void MaxManaChanged(const FOnAttributeChangeData& AttributeData) const;
	void StaminaChanged(const FOnAttributeChangeData& AttributeData) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& AttributeData) const;
};
